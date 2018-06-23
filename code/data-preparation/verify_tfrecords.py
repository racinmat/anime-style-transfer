"""Checks if a set of TFRecords appear to be valid.

Specifically, this checks whether the provided record sizes are consistent and
that the file does not end in the middle of a record. It does not verify the
CRCs.

Run as: python3 verify_tfrecords.py --input_data_pattern=/mnt/datagrid/personal/racinmat/anime-style-transfer/datasets/anime/*.tfrecord
or as: python3 verify_tfrecords.py --input_data_pattern=/mnt/datagrid/personal/racinmat/anime-style-transfer/datasets/real/*.tfrecord
"""
import struct
import tensorflow as tf
from google.protobuf.descriptor import FieldDescriptor
from tensorflow import gfile
from tensorflow import logging
from tensorflow.core.example.example_pb2 import Example
from tensorflow.core.example.feature_pb2 import Features

FLAGS = tf.flags.FLAGS
tf.flags.DEFINE_string("input_data_pattern", "",
                       "File glob defining for the TFRecords files.")


def main(_):
    logging.set_verbosity(tf.logging.INFO)
    print('input_data_pattern is: {}'.format(FLAGS.input_data_pattern))
    paths = gfile.Glob(FLAGS.input_data_pattern)
    logging.info("Found %s files.", len(paths))
    for path in paths:
        for bin_dump in tf.python_io.tf_record_iterator(path):
            example = tf.train.Example.FromString(bin_dump)  # type: Example
            print('record type:')
            print(type(example))
            print('methods and stuff')
            print(dir(example))
            print('record fields:')
            field = None  # type: FieldDescriptor
            features = None  # type: Features
            for field, features in example.ListFields():
                print(type(field), type(features))
                # print('field methods and stuff')
                # print(dir(field))
                print('field type:', field.type)
                print('field name:', field.name)
                print('field label:', field.label)
                # print('features methods and stuff')
                # print(dir(features))
                print('features list dump:')
                for field_2, features_2 in features.ListFields():
                    print(type(field_2), type(features_2))
                    print('field_2 type:', field_2.type)
                    print('field_2 name:', field_2.name)
                    print('field_2 label:', field_2.label)
                    # print('features_2 methods and stuff')
                    # print(dir(features_2))
                    print('features_2 keys')
                    print(list(features_2.keys()))

            break


if __name__ == "__main__":
    tf.app.run()
