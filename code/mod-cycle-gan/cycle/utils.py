import random
import logging
import os
import os.path as osp
import tensorflow as tf
import numpy as np


def _identity(x, name=None):
    return tf.identity(x, name=name)


class DataBuffer:
    def __init__(self, pool_size, batch_size, old_prob=0.5):
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
    def __init__(self, tfrecords_file, name, shape, shuffle_buffer_size=100, normer=_identity, denormer=_identity,
                 batch_size=1, num_threads=8):
        self.name = name
        self.batch_size = batch_size
        self.num_threads = num_threads
        self.shape = shape
        self.normalize = normer
        self.denormalize = denormer
        with tf.name_scope(self.name):
            print('fetching data from tfrecords_file: {}'.format(tfrecords_file))
            print('with name: {}'.format(self.name))
            self.data = tf.data.TFRecordDataset(tfrecords_file)
            self.data = self.data.map(self._parse_example, num_parallel_calls=num_threads)
            self.data = self.data.map(self.normalize, num_parallel_calls=num_threads)
            self.data = self.data.shuffle(shuffle_buffer_size)
            self.data = self.data.repeat()
            self.data = self.data.batch(self.batch_size)
            self.iterator = self.data.make_one_shot_iterator()
        self.feeder = self.iterator.get_next()

    def feed(self):
        return self.feeder

    def _parse_example(self, serialized_example):
        name = self.name + '_data'
        features = tf.parse_single_example(serialized_example,
                                           {name: tf.FixedLenFeature(self.shape, tf.float32)})
        return features[name]


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
