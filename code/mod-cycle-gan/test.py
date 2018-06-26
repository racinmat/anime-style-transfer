#!/usr/bin/env python3

import os
import os.path as osp
import logging
import tensorflow as tf
import cycle
import numpy as np

FLAGS = tf.flags.FLAGS

tf.flags.DEFINE_string('Xin', '/datagrid/personal/jasekota/dip-dataset/valeo/valeo-tst.npy', 'Name of the X input npy file.')
tf.flags.DEFINE_string('Yin', '/datagrid/personal/jasekota/dip-dataset/gta/gta-tst.npy', 'Name of the Y input npy file.')
tf.flags.DEFINE_string('XYout', '/datagrid/personal/jasekota/dip-dataset/valeo/valeo-tst-out.npz', 'Name of the X converted to Y output npz file.')
tf.flags.DEFINE_string('YXout', '/datagrid/personal/jasekota/dip-dataset/gta/gta-tst-out.npz', 'Name of the Y converted to X output npz file.')
tf.flags.DEFINE_bool('includein', True, 'Whether to include input data in the output file. If on, output file will be larger, but self-contained.')


def main(_):
    logging.getLogger().setLevel(logging.INFO)
    if FLAGS.rundir is None:
        FLAGS.rundir = sorted([d for d in os.listdir(FLAGS.cpdir)
                               if osp.isdir(osp.join(FLAGS.cpdir, d))])[-1]
    fulldir = osp.join(FLAGS.cpdir, FLAGS.rundir)
    in_data = np.load(FLAGS.Xin, mmap_mode='r')
    d_inputs, d_outputs, outputs = cycle.CycleGAN.test_one_part(osp.join(fulldir, '{}2{}.pb'.format(FLAGS.Xname, FLAGS.Yname)),
                                 in_data)
    cycle.CycleGAN.save_output(in_data, d_inputs, d_outputs, FLAGS.includein, FLAGS.XYout, outputs)
    in_data = np.load(FLAGS.Yin, mmap_mode='r')
    d_inputs, d_outputs, outputs = cycle.CycleGAN.test_one_part(osp.join(fulldir, '{}2{}.pb'.format(FLAGS.Yname, FLAGS.Xname)),
                                 in_data)
    cycle.CycleGAN.save_output(in_data, d_inputs, d_outputs, FLAGS.includein, FLAGS.YXout, outputs)


if __name__ == '__main__':
    tf.app.run()
