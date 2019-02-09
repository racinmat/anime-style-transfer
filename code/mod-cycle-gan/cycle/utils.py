import random
import logging
import os
import os.path as osp
import tensorflow as tf
import numpy as np
from object_detection.data_decoders import tf_example_decoder

from data_preparation.images_to_tfrecord import process_sample_tf


def _identity(x, name=None):
    return tf.identity(x, name=name)


class DataBuffer:
    def __init__(self, pool_size, batch_size, old_prob=0.5, name=None):
        self.name = name
        self.pool_size = pool_size
        self.batch_size = batch_size
        assert self.pool_size >= self.batch_size or self.pool_size == -1
        self.prob = old_prob
        self.data = []
        self.last_data = None
        self.last_step = -1

    def query(self, data, cur_step):
        if cur_step == self.last_step and self.last_data is not None:
            return self.last_data
        assert data.shape[0] == self.batch_size
        self.last_data = np.empty_like(data)
        self.last_step = cur_step
        if self.pool_size == -1:
            self.last_data = data
        elif self.batch_size == 1:
            # special case, support for dynamic shapes, duplicate code, but so far without ideas for refactoring
            if len(self.data) < self.pool_size:
                self.data.append(data[0].copy())
                self.last_data = data
            else:
                to_return = random.random()
                to_replace = random.random()
                if to_return > self.prob:
                    self.last_data = data
                else:
                    idx = random.randrange(0, self.pool_size)
                    self.last_data = self.data[idx][np.newaxis, :]
                if to_replace > self.prob:
                    idx = random.randrange(0, self.pool_size)
                    self.data[idx] = data[0]
        else:
            for i, d in enumerate(data):
                if len(self.data) < self.pool_size:
                    self.data.append(d.copy())
                    self.last_data[i] = d
                else:
                    to_return = random.random()
                    to_replace = random.random()
                    if to_return > self.prob:
                        self.last_data[i] = d
                    else:
                        idx = random.randrange(0, self.pool_size)
                        self.last_data[i] = self.data[idx]
                    if to_replace > self.prob:
                        idx = random.randrange(0, self.pool_size)
                        self.data[idx] = d
        return self.last_data


class TFReader:
    def __init__(self, tfrecords_file, shuffle_buffer_size=100, normer=_identity, denormer=_identity,
                 batch_size=1, num_threads=8):
        self.batch_size = batch_size
        self.num_threads = num_threads
        self.normalize = normer
        self.denormalize = denormer
        with tf.device('/cpu:0'):
            with tf.name_scope('tfreader'):
                print('fetching data from tfrecords_file: {}'.format(tfrecords_file))
                self.data = tf.data.TFRecordDataset(tfrecords_file)
                self.data = self.data.map(self._parse_example_encoded, num_parallel_calls=num_threads)
                self.data = self.data.map(self.normalize, num_parallel_calls=num_threads)
                self.data = self.data.map(self._reshape_to_even, num_parallel_calls=num_threads)
                self.data = self.data.apply(tf.data.experimental.shuffle_and_repeat(shuffle_buffer_size))
                self.data = self.data.batch(self.batch_size)
                self.data = self.data.prefetch(4)
                self.iterator = self.data.make_one_shot_iterator()
        self.feeder = self.iterator.get_next()

    def feed(self):
        return self.feeder

    @staticmethod
    def _parse_example_encoded(serialized_example):
        example_decoder = tf_example_decoder.TfExampleDecoder()
        features = example_decoder.decode(tf.convert_to_tensor(serialized_example))
        image = process_sample_tf(features['image'], False)
        return tf.cast(image, dtype=tf.float32)

    @staticmethod
    def _reshape_to_even(image):
        # because down and up scaling in network breaks dimensions for odd shaped input images
        # because this is max. 1 row and 1 column correction, I can use symmetry and won't change input statistics much
        im_shape = tf.shape(image)
        nearest_even_shape = tf.to_int32(tf.ceil(im_shape / 2) * 2)
        diff_shape = nearest_even_shape - im_shape
        y_diff = diff_shape[0]
        x_diff = diff_shape[1]
        return tf.pad(image, [[0, y_diff], [0, x_diff], [0, 0]], 'SYMMETRIC')


class TFWriter:
    def __init__(self, name, infiles, process_data=lambda x: x):

        if not isinstance(infiles, list):
            self.infiles = [infiles]
        else:
            self.infiles = infiles
        self.name = name
        self.process_data = process_data

    def run(self, outfile):
        if osp.isfile(outfile):
            logging.warning('File %s already exists, it will be overwritten', outfile)
        outfile = osp.abspath(outfile)
        os.makedirs(os.path.dirname(outfile), exist_ok=True)
        writer = tf.python_io.TFRecordWriter(outfile)
        name = self.name + '_data'
        for f in self.infiles:
            logging.info('Opening %s', f)
            try:
                data = np.load(f, mmap_mode='r')
                for j, d in enumerate(data):
                    if j % 10 == 0:
                        logging.info('%d / %d : \t %s', j, data.shape[0], f)
                    ex = tf.train.Example(features=tf.train.Features(
                        feature={
                            name: tf.train.Feature(float_list=
                                                   tf.train.FloatList(value=
                                                                      self.process_data(d).ravel()))
                        }))
                    writer.write(ex.SerializeToString())
            except (IOError, ValueError) as e:
                logging.warning('Opening %s failed', f)
                logging.warning(e)
        writer.close()


# both functions are for visualizing tensorflow graph in tensor-board like way without actually starting tensorboard
# copied from https://github.com/yaroslavvb/stuff/blob/master/simple_rewiring.ipynb
def strip_consts(graph_def, max_const_size=32):
    """Strip large constant values from graph_def."""
    strip_def = tf.GraphDef()
    for n0 in graph_def.node:
        n = strip_def.node.add()
        n.MergeFrom(n0)
        if n.op == 'Const':
            tensor = n.attr['value'].tensor
            size = len(tensor.tensor_content)
            if size > max_const_size:
                tensor.tensor_content = "<stripped %d bytes>" % size
    return strip_def


def show_graph(graph_def=None, max_const_size=32, ungroup_gradients=False):
    if not graph_def:
        graph_def = tf.get_default_graph().as_graph_def()

    """Visualize TensorFlow graph."""
    if hasattr(graph_def, 'as_graph_def'):
        graph_def = graph_def.as_graph_def()
    strip_def = strip_consts(graph_def, max_const_size=max_const_size)
    data = str(strip_def)
    if ungroup_gradients:
        data = data.replace('"gradients/', '"b_')
        # print(data)
    code = """
    <html>
        <script>
          function load() {{
            document.getElementById("{id}").pbtxt = {data};
          }}
        </script>
        <link rel="import" href="https://tensorboard.appspot.com/tf-graph-basic.build.html" onload=load()>
        <div>
          <tf-graph-basic id="{id}"></tf-graph-basic>
        </div>
    </html>
    """.format(data=repr(data), id='graph' + str(np.random.rand()))

    with open('graph.html', mode='w+') as f:
        f.write(code)
