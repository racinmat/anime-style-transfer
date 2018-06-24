#!/usr/bin/env python3

import os
import os.path as osp
import logging
import random

import tensorflow as tf
import cycle
from train import create_cyclegan, initialize_networks


FLAGS = tf.flags.FLAGS

tf.flags.DEFINE_string('Xin', '/datagrid/personal/jasekota/dip-dataset/valeo/valeo-tst.npy',
                       'Name of the X input npy file.')
tf.flags.DEFINE_string('Yin', '/datagrid/personal/jasekota/dip-dataset/gta/gta-tst.npy',
                       'Name of the Y input npy file.')
tf.flags.DEFINE_string('XYout', '/datagrid/personal/jasekota/dip-dataset/valeo/valeo-tst-out.npz',
                       'Name of the X converted to Y output npz file.')
tf.flags.DEFINE_string('YXout', '/datagrid/personal/jasekota/dip-dataset/gta/gta-tst-out.npz',
                       'Name of the Y converted to X output npz file.')
tf.flags.DEFINE_bool('includein', True,
                     'Whether to include input data in the output file. If on, output file will be larger, but self-contained.')


def load_and_export(checkpoint_dir, export_dir):
    modellib, xfeed, xy, yfeed, yx = initialize_networks()

    XtoY = xy
    YtoX = yx

    X_normer = xfeed.normalize
    Y_normer = yfeed.normalize
    X_denormer = xfeed.denormalize
    Y_denormer = yfeed.denormalize

    cycle.CycleGAN.export_from_checkpoint(XtoY, YtoX, X_normer, X_denormer, Y_normer, Y_denormer,
                                          checkpoint_dir, export_dir, FLAGS.Xname, FLAGS.Yname)


def main(_):
    # just as test, but with loading from checkpoint
    in_images = [
        ''
    ]
    # random.sample(get_real_images(), 50)
    logging.getLogger().setLevel(logging.INFO)

    if FLAGS.rundir is None:
        FLAGS.rundir = sorted([d for d in os.listdir(FLAGS.cpdir)
                               if osp.isdir(osp.join(FLAGS.cpdir, d))])[-1]
    fulldir = osp.join(FLAGS.cpdir, FLAGS.rundir)

    load_and_export(fulldir, osp.join(FLAGS.cpdir, '..', 'export'))

    cycle.CycleGAN.test_one_part(osp.join(fulldir, '{}2{}.pb'.format(FLAGS.Xname, FLAGS.Yname)),
                                 FLAGS.Xin, FLAGS.XYout, FLAGS.includein)
    cycle.CycleGAN.test_one_part(osp.join(fulldir, '{}2{}.pb'.format(FLAGS.Yname, FLAGS.Xname)),
                                 FLAGS.Yin, FLAGS.YXout, FLAGS.includein)


if __name__ == '__main__':
    tf.app.run()
