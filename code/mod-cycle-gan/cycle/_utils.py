import random
import tensorflow as tf
import numpy as np

def convert2int(image):
    return tf.image.convert_image_dtype((image+1.0)/2.0, tf.uint8)

def convert2float(image):
    image = tf.image.convert_image_dtype(image, dtype=tf.float32)
    return (image/127.5) - 1.0

def batch_convert2int(images, name=None):
    return tf.map_fn(convert2int, images, dtype=tf.uint8, name=name)

def batch_convert2float(images, name=None):
    return tf.map_fn(convert2float, images, dtype=tf.float32, name=name)

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
                    p = random.random()
                    if p > self.prob:
                        self.last_data[i] = d
                    else:
                        idx = random.randrange(0, self.pool_size)
                        self.last_data[i] = self.data[idx]
                        self.data[idx] = d
        return self.last_data

class TFReader:
    def __init__(self, tfrecords_file, name, shape, batch_size=1, num_threads=8):
        """
        Args:
          tfrecords_file: string, tfrecords file path
          min_queue_examples: Number of examples to retain in the queue (tf param)
          batch_size: integer, number of images per batch
          num_threads: integer, number of preprocess threads
        """

        self.name = name
        self.batch_size = batch_size
        self.num_threads = num_threads
        self.shape = shape
        with tf.name_scope(self.name):
            self.data = tf.data.TFRecordDataset(tfrecords_file)
            self.data = self.data.map(self._parse_example, num_parallel_calls=num_threads)
            self.data = self.data.map(self.normalize, num_parallel_calls=num_threads)
            self.data = self.data.shuffle()
            self.data = self.data.repeat()
            self.data = self.data.batch(self.batch_size)
            self.iterator = self.data.make_one_shot_iterator()


    def feed(self):
        """
        Returns:
          images: 4D tensor [batch_size, *shape]
        """
        return self.iterator.get_next()

    def _parse_example(self, serialized_example):
        name = self.name + '_transform'
        features = tf.parse_single_example(serialized_example,
                                           {name: tf.FixedLenFeature(self.shape, tf.float32)})
        return features[name]

    @staticmethod
    def normalize(example, name=''):
        return tf.identity(example, name=name)

    @staticmethod
    def denormalize(example, name=''):
        return tf.identity(example, name=name)
