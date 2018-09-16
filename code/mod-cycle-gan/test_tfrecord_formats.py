import unittest
import tensorflow as tf
import logging
import glob
import numpy as np
from PIL import Image
from object_detection.utils import dataset_util
import numpy.testing as npt
from object_detection.data_decoders import tf_example_decoder


class MyTestCase(unittest.TestCase):

    def create_tfrecord_array(self, outfile, infiles):
        writer = tf.python_io.TFRecordWriter(outfile)

        for i, f in enumerate(infiles):
            data = np.array(Image.open(f), dtype=np.float32)
            ex = tf.train.Example(features=tf.train.Features(
                feature={'image': tf.train.Feature(float_list=tf.train.FloatList(value=data.ravel()))}))
            writer.write(ex.SerializeToString())
        writer.close()

    def create_tfrecord_encoded(self, outfile, infiles):
        writer = tf.python_io.TFRecordWriter(outfile)

        for i, f in enumerate(infiles):
            with tf.gfile.GFile(f, 'rb') as fid:
                encoded_png = fid.read()
            ex = tf.train.Example(features=tf.train.Features(feature={
                'image/encoded': dataset_util.bytes_feature(encoded_png),
                'image/format': dataset_util.bytes_feature('png'.encode('utf8')),
                'image/source_id': dataset_util.bytes_feature('image_id'.encode('utf8')),
            }))
            writer.write(ex.SerializeToString())
        writer.close()

    def load_tfrecord_array(self, tfrecord_file):
        data = tf.data.TFRecordDataset(tfrecord_file)
        data = data.map(self.parse_example_array)
        iterator = data.make_one_shot_iterator()
        feeder = iterator.get_next()

        images = []
        with tf.Session(config=tf.ConfigProto(device_count={'GPU': 0})) as sess:
            for i in range(10):
                images.append(sess.run(feeder))
        return images

    def parse_example_array(self, serialized_example):
        shape = (1080, 1920, 3)
        features = tf.parse_single_example(serialized_example, {'image': tf.FixedLenFeature(shape, tf.float32)})
        return features['image']
        # return features

    def load_tfrecord_encoded(self, tfrecord_file):
        data = tf.data.TFRecordDataset(tfrecord_file)
        data = data.map(self.parse_example_encoded)
        iterator = data.make_one_shot_iterator()
        feeder = iterator.get_next()

        images = []
        with tf.Session(config=tf.ConfigProto(device_count={'GPU': 0})) as sess:
            for i in range(10):
                images.append(sess.run(feeder))
        return images

    def parse_example_encoded(self, serialized_example):
        example_decoder = tf_example_decoder.TfExampleDecoder()
        features = example_decoder.decode(tf.convert_to_tensor(serialized_example))
        return features['image']

    def test_png_tfrecord_sizes(self):
        infiles = list(glob.glob('test_data/png/*.png', recursive=True))
        orig_images = [np.array(Image.open(f), dtype=np.float32) for f in infiles]

        self.create_tfrecord_array('test_data/temp/images_array.record', infiles)
        self.create_tfrecord_encoded('test_data/temp/images_encoded.record', infiles)

        array_images = self.load_tfrecord_array('test_data/temp/images_array.record')
        encoded_images = self.load_tfrecord_encoded('test_data/temp/images_encoded.record')
        npt.assert_equal(orig_images, array_images)
        npt.assert_equal(orig_images, encoded_images)


if __name__ == '__main__':
    unittest.main()
