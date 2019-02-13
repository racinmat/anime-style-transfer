#!/usr/bin/env python3

"""

Script for neural network inference. Transforms real images into anime.
Inference from anime into real world is possible, but not implemented, because no one cares about real world.

Params:
    --rundir=path   directory with checkpoints for trained model
    --flagfile=path flagfile with run configuration

Examples:
    python transform.py --rundir=2019-01-05--00-48-0 --flagfile=checkpoint/2019-01-05--00-48-0/params.flagfile
"""

import glob
import os
import os.path as osp
import logging
import random
from math import ceil

import numpy as np
import progressbar
import tensorflow as tf
from PIL import Image
from joblib import Parallel, delayed
from scipy.misc import imsave
import cycle
from cycle.models.anime import X_DATA_SHAPE
from train import initialize_networks, import_model
from data_preparation.images_to_tfrecord import get_real_images_ade20k, process_sample_tf
from transform import load_and_export

FLAGS = tf.flags.FLAGS


def main(_):
    logging.getLogger().setLevel(logging.INFO)
    # just as test, but with loading from checkpoint
    extract_and_get_pb_dir()
    print('extraction done')


def extract_and_get_pb_dir():
    if FLAGS.rundir is None:
        FLAGS.rundir = sorted([d for d in os.listdir(FLAGS.cpdir)
                               if osp.isdir(osp.join(FLAGS.cpdir, d))])[-1]
    fulldir = osp.join(FLAGS.cpdir, FLAGS.rundir)
    print('rundir: ', FLAGS.rundir)
    pb_dir = osp.join(FLAGS.cpdir, '..', 'export', FLAGS.rundir)
    if not osp.exists(pb_dir):
        os.makedirs(pb_dir)
    load_and_export(fulldir, pb_dir)


if __name__ == '__main__':
    tf.app.run()
