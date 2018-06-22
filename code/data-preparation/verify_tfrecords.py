"""Checks if a set of TFRecords appear to be valid.

Specifically, this checks whether the provided record sizes are consistent and
that the file does not end in the middle of a record. It does not verify the
CRCs.

Run as: python3 verify_tfrecords.py --input_data_pattern=/mnt/datagrid/personal/racinmat/anime-style-transfer/datasets/anime/*.tfrecord
"""
import struct
import tensorflow as tf

from tensorflow import gfile
from tensorflow import logging

from google.protobuf.json_format import MessageToJson

FLAGS = tf.flags.FLAGS
tf.flags.DEFINE_string("input_data_pattern", "",
                       "File glob defining for the TFRecords files.")


def main(_):
    logging.set_verbosity(tf.logging.INFO)
    print('input_data_pattern is: {}'.format(FLAGS.input_data_pattern))
    paths = gfile.Glob(FLAGS.input_data_pattern)
    logging.info("Found %s files.", len(paths))
    for path in paths:
        for example in tf.python_io.tf_record_iterator(path):
            print('record:')
            print(example)
            # result = tf.train.Example.FromString(example)
            # jsonMessage = MessageToJson(result)
            # print('tfrecord file contents: ')
            # print(jsonMessage)


if __name__ == "__main__":
    tf.app.run()
