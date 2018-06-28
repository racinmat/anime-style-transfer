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

Examples:
    python transform.py --random=20    # takes 20 random images from ade20k dataset and transforms them
    python transform.py --inpath=../images/*.png    # takes all images from specified dir and transforms them
    python transform.py --inpath=../images/*.jpg --includein    # takes all images from specified dir and transforms them, including input images
    python transform.py --inpath=../../data/images/20180625-1659-0/20000/*-in.png --extract
    python transform.py --inpath=../../data/images/shizu/*.jpg --outdir=../../data/images/shizu/out --includein=0
    python transform.py --inpath=../../dataset-sources/real/images/animefest-2016/*.png --outdir=../../data/images/animefest-2016 --includein=0
    python transform.py --random=30 --outdir=../../data/images/ade20k --includein=1
    python transform.py --inpath=../../dataset-sources/real/images/animefest-2017-cosplay/*.png --outdir=../../data/images/animefest-2017-cosplay --includein=0

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
from cycle.models.anime import X_DATA_SHAPE
from train import initialize_networks, import_model
from data_preparation.images_to_tfrecord import process_sample, get_real_images_ade20k

FLAGS = tf.flags.FLAGS

tf.flags.DEFINE_bool('extract', False, 'if true, extraction of newest checkpoint will be performed. Needs tensorflow checkpoints')
tf.flags.DEFINE_string('inpath', None, 'path to input images, with unix wildcards')
tf.flags.DEFINE_integer('random', 20, 'If not none, random images are taken')
tf.flags.DEFINE_string('outdir', '../../data/images', 'Name of output dir')
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

    step = cycle.CycleGAN.export_from_checkpoint(XtoY, YtoX, X_normer, X_denormer, Y_normer, Y_denormer,
                                                 checkpoint_dir, export_dir, FLAGS.Xname, FLAGS.Yname)
    return step


def images_to_numpy(im_paths):
    # show progressbar for more images and use parallelism. It is not needed for small amount of data, but very useful for bigger amount.
    if len(im_paths) > 100:
        return images_to_numpy_parallel(im_paths)
    else:
        return images_to_numpy_simple(im_paths)


def images_to_numpy_simple(im_paths):
    one_img_size = X_DATA_SHAPE
    data = np.zeros((len(im_paths), one_img_size[0], one_img_size[1], one_img_size[2]), dtype=np.float32)

    # show progressbar for more images
    for i, f in enumerate(im_paths):
        try:
            image = process_sample(np.array(Image.open(f)), True)
            data[i, :, :, :] = image
        except OSError:
            data[i, :, :, :] = data[i-1, :, :, :]   # just use previous sample if anything goes wrong

    return data


def one_image_to_numpy(i, f, pbar, data, counter):
    pbar.update(counter[0])
    counter[0] += 1
    try:
        image = process_sample(np.array(Image.open(f)), True)
        data[i, :, :, :] = image
    except OSError:
        data[i, :, :, :] = data[i - 1, :, :, :]  # just use previous sample if anything goes wrong


def images_to_numpy_parallel(im_paths):
    one_img_size = X_DATA_SHAPE
    data = np.zeros((len(im_paths), one_img_size[0], one_img_size[1], one_img_size[2]), dtype=np.float32)

    widgets = [progressbar.Percentage(), ' ', progressbar.Counter(), ' ', progressbar.Bar(), ' ',
               progressbar.FileTransferSpeed()]
    pbar = progressbar.ProgressBar(widgets=widgets, max_value=len(im_paths)).start()

    counter = [0]
    workers = 10

    Parallel(n_jobs=workers, backend='threading')(
        delayed(one_image_to_numpy)(i, f, pbar, data, counter) for i, f in enumerate(im_paths))

    pbar.finish()

    return data


def numpy_to_images(data, out_dir, suffix='-out'):
    if not osp.exists(out_dir):
        os.makedirs(out_dir)
    print('going to persist {} images'.format(len(data)))
    num_digits = int(np.log10(len(data)))+1

    widgets = [progressbar.Percentage(), ' ', progressbar.Counter(), ' ', progressbar.Bar(), ' ',
               progressbar.FileTransferSpeed()]
    pbar = progressbar.ProgressBar(widgets=widgets, max_value=len(data)).start()

    for i, im in enumerate(data):
        pbar.update(i)
        imsave(osp.join(out_dir, '{}{}.png'.format(str(i).zfill(num_digits), suffix)), im)
    pbar.finish()


def main(_):
    logging.getLogger().setLevel(logging.INFO)
    # just as test, but with loading from checkpoint
    if FLAGS.inpath is not None:
        print('using images in path: ', FLAGS.inpath)
        im_paths = list(sorted(glob.glob(FLAGS.inpath)))
    elif FLAGS.random is not None:
        num_images = FLAGS.random
        print('using {} random images from ade20k'.format(num_images))
        im_paths = random.sample(get_real_images_ade20k(), num_images)
    else:
        raise Exception("No data source specified")

    print('will transform {} images: '.format(len(im_paths)))
    in_data = images_to_numpy(im_paths)
    print('images prepared in numpy')

    if FLAGS.extract:
        if FLAGS.rundir is None:
            FLAGS.rundir = sorted([d for d in os.listdir(FLAGS.cpdir)
                                   if osp.isdir(osp.join(FLAGS.cpdir, d))])[-1]
        fulldir = osp.join(FLAGS.cpdir, FLAGS.rundir)

        print('rundir: ', FLAGS.rundir)
        pb_dir = osp.join(FLAGS.cpdir, '..', 'export', FLAGS.rundir)
        if not osp.exists(pb_dir):
            os.makedirs(pb_dir)
        step = load_and_export(fulldir, pb_dir)
        rundir = FLAGS.rundir
    else:
        import_model()      # model is imported during load_and_export in other case
        pb_dir = osp.join(FLAGS.cpdir, '..', 'export')
        rundir = sorted([d for d in os.listdir(pb_dir) if osp.isdir(osp.join(pb_dir, d))])[-1]
        full_rundir = osp.join(pb_dir, rundir)
        print('rundir:', rundir)
        step = str(max([int(d) for d in os.listdir(full_rundir) if osp.isdir(osp.join(full_rundir, d))]))
        pb_dir = osp.join(pb_dir, rundir)

    print('model loaded, starting with inference')

    d_inputs, d_outputs, outputs = cycle.CycleGAN.test_one_part(
        osp.join(pb_dir, step, '{}2{}.pb'.format(FLAGS.Xname, FLAGS.Yname)),
        in_data)

    print('data transformed, going to persist them')

    if FLAGS.includein:
        numpy_to_images(in_data, osp.join(FLAGS.outdir, rundir, step), suffix='-in')
    numpy_to_images(outputs, osp.join(FLAGS.outdir, rundir, step), suffix='-out')
    print('all done')


if __name__ == '__main__':
    tf.app.run()
