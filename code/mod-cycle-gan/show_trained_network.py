#!/usr/bin/env python3

"""
Script for neural network inference. Transforms real images into anime.
Inference from anime into real world is possible, but not implemented, because no one cares about real world.

Params:
    --includein   includes original images into output data
    --random=number   how many random images from ade20k dataset will be taken
    --inpath=path  path to dir with input images
    --outdir=path    dir for output, where dir with network name is created with images in it
    --rundir=path     dir with model checkpoint. If not specified, last network is taken

"""

import glob
import os
import os.path as osp
import logging
import random
import numpy as np
import progressbar
import tensorflow as tf
from PIL import Image
from joblib import Parallel, delayed
from scipy.misc import imsave
import cycle
from cycle import CycleGAN
from cycle.models.anime import X_DATA_SHAPE
from train import initialize_networks, import_model
from data_preparation.images_to_tfrecord import process_sample, get_real_images_ade20k

FLAGS = tf.flags.FLAGS

tf.flags.DEFINE_string('inpath', None, 'path to input images, with unix wildcards')
tf.flags.DEFINE_integer('random', 20, 'If not none, random images are taken')
tf.flags.DEFINE_string('outdir', '../../data/images', 'Name of output dir')
tf.flags.DEFINE_bool('includein', True,
                     'Whether to include input data in the output file. If on, output file will be larger, but self-contained.')


def extract_to_tensorboard():
    pb_dir, rundir, step = get_pb_dir()

    pb_model_dir = osp.join(pb_dir, step)
    pb_model = osp.join(pb_model_dir, '{}2{}.pb'.format(FLAGS.Xname, FLAGS.Yname))

    graph = tf.get_default_graph()
    with graph.as_default():
        graph_def = tf.GraphDef()
        with tf.gfile.FastGFile(pb_model, 'rb') as model_file:
            graph_def.ParseFromString(model_file.read())

    tf.import_graph_def(graph_def)
    train_writer = tf.summary.FileWriter(pb_model_dir)
    train_writer.add_graph(graph)
    train_writer.flush()
    train_writer.close()


def get_pb_dir():
    pb_dir = osp.join(FLAGS.cpdir, '..', 'export')
    rundir = FLAGS.rundir
    full_rundir = osp.join(pb_dir, rundir)
    print('rundir:', rundir)
    step = str(max([int(d) for d in os.listdir(full_rundir) if osp.isdir(osp.join(full_rundir, d))]))
    pb_dir = osp.join(pb_dir, rundir)
    return pb_dir, rundir, step


def main(_):
    logging.getLogger().setLevel(logging.INFO)
    # just as test, but with loading from checkpoint
    extract_to_tensorboard()
    print('all done')


if __name__ == '__main__':
    tf.app.run()
