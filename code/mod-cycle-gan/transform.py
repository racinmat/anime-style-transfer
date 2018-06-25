#!/usr/bin/env python3

import os
import os.path as osp
import logging
import random
import numpy as np
import tensorflow as tf
from PIL import Image
from scipy.misc import imsave

import cycle
from cycle.models.anime import X_DATA_SHAPE
from train import initialize_networks
from data_preparation.images_to_tfrecord import get_real_images_cityscapes, process_sample

FLAGS = tf.flags.FLAGS

tf.flags.DEFINE_string('Xin', '/datagrid/personal/racinmat/anime-style-transfer/data/real_in.npy',
                       'Name of the X input npy file.')
# tf.flags.DEFINE_string('Yin', '/datagrid/personal/jasekota/dip-dataset/gta/gta-tst.npy',
#                        'Name of the Y input npy file.')
# tf.flags.DEFINE_string('XYout', '/datagrid/personal/racinmat/anime-style-transfer/data/anime_out.npy',
#                        'Name of the X converted to Y output npz file.')
tf.flags.DEFINE_string('XYout', '/datagrid/personal/racinmat/anime-style-transfer/data/images',
                       'Name of the X converted to Y output npz file.')
# tf.flags.DEFINE_string('YXout', '/datagrid/personal/jasekota/dip-dataset/gta/gta-tst-out.npz',
#                        'Name of the Y converted to X output npz file.')
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


def images_to_numpy(im_paths, out_path):
    one_img_size = X_DATA_SHAPE
    data = np.zeros((len(im_paths), one_img_size[0], one_img_size[1], one_img_size[2]), dtype=np.float32)
    for i, f in enumerate(im_paths):
        image = process_sample(np.array(Image.open(f)))
        data[i, :, :, :] = image
    np.save(out_path, data)


def numpy_to_images(data, out_dir, suffix='-out'):
    for i, im in enumerate(data):
        # im = data[i, :, :, :]
        imsave(osp.join(out_dir, '{}{}.png'.format(i, suffix)), im)


def main(_):
    # just as test, but with loading from checkpoint
    num_images = 20
    im_paths = random.sample(get_real_images_cityscapes(), num_images)
    images_to_numpy(im_paths, FLAGS.Xin)

    logging.getLogger().setLevel(logging.INFO)

    if FLAGS.rundir is None:
        FLAGS.rundir = sorted([d for d in os.listdir(FLAGS.cpdir)
                               if osp.isdir(osp.join(FLAGS.cpdir, d))])[-1]
    fulldir = osp.join(FLAGS.cpdir, FLAGS.rundir)

    pb_dir = osp.join(FLAGS.cpdir, '..', 'export')
    load_and_export(fulldir, pb_dir)

    all_data, d_inputs, d_outputs, outputs = cycle.CycleGAN.test_one_part(osp.join(pb_dir, '{}2{}.pb'.format(FLAGS.Xname, FLAGS.Yname)),
                                 FLAGS.Xin, FLAGS.XYout, FLAGS.includein)
    # cycle.CycleGAN.test_one_part(osp.join(fulldir, '{}2{}.pb'.format(FLAGS.Yname, FLAGS.Xname)),
    #                              FLAGS.Yin, FLAGS.YXout, FLAGS.includein)

    numpy_to_images(all_data, FLAGS.XYout, suffix='-in')
    print(type(outputs))
    print(len(outputs))
    print(outputs)
    numpy_to_images(outputs, FLAGS.XYout, suffix='-out')
    print('all done')


if __name__ == '__main__':
    tf.app.run()
