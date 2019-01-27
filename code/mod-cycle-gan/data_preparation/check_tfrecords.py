import os.path as osp
import tensorflow as tf
from object_detection.data_decoders import tf_example_decoder
from tensorflow.python.framework.errors_impl import OutOfRangeError, DataLossError

from cycle.utils import TFReader
from data_preparation.images_to_tfrecord import process_sample_tf

FLAGS = tf.flags.FLAGS

# others flags are in check_tfrecords_pure
tf.flags.DEFINE_string('file', None, 'TFRecord to check.')
tf.flags.DEFINE_bool('no_fail', False, 'If continue even after corrupted data is found.')

'''
This script checks if the tfrecord is correct. 
So far, only pixelwise mask is checked.
Run by:
python check_tfrecords.py --file=data/datasets/saint_gobain
'''


def check_record(data):
    pass


def check_feeder(sess, feeder, no_fail):
    failed_data = []
    all_count = 0
    try:
        while True:
            all_count += 1
            try:
                data = sess.run(feeder)
                check_record(data)
            except (AssertionError, DataLossError) as e:
                if no_fail:
                    failed_data.append(str(e))
                    print(str(e))
                else:
                    raise e
    except OutOfRangeError:
        print('end of data')
        pass
    return failed_data, all_count


def main(_):
    file = FLAGS.file
    no_fail = FLAGS.no_fail
    num_threads = 10
    print(file)
    example_decoder = tf_example_decoder.TfExampleDecoder()
    dataset = tf.data.TFRecordDataset(file)
    dataset = dataset.map(TFReader._parse_example_encoded, num_parallel_calls=num_threads)
    # dataset = dataset.map(tf.convert_to_tensor, num_parallel_calls=num_threads)
    # dataset = dataset.map(example_decoder.decode, num_parallel_calls=num_threads)
    iterator = dataset.make_one_shot_iterator()
    feeder = iterator.get_next()

    sess = tf.Session(config=tf.ConfigProto(device_count={'GPU': 0}))  # don't need GPU to check the dataset

    failed, all_count = check_feeder(sess, feeder, no_fail)
    if failed:
        print('there is {} faulty samples of {} in data'.format(len(failed), all_count))
        print('failures info:')
        print(failed)
    else:
        print('No error in data of {} samples.'.format(all_count))

    # for example in tf.python_io.tf_record_iterator(file):
    #     result = tf.train.Example.FromString(example)


if __name__ == '__main__':
    tf.app.run()
