#!/usr/bin/env python3

# TODO: parametrizable main

import tensorflow as tf
import cycle
import cycle.models.lidar as lidar

FLAGS = tf.flags.FLAGS

tf.flags.DEFINE_string('valeo-tfr', '/datagrid/personal/jasekota/dip-dataset/valeo/valeo-trn.tfrecords', '')
tf.flags.DEFINE_string('gta-tfr', '/datagrid/personal/jasekota/dip-dataset/gta/gta-trn.tfrecords', '')
tf.flags.DEFINE_string('gan-type', 'wgan', '')
