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
    python transform.py --inpath=../../data/images/20180625-1659-0/20000/*-in.png --extract     # that does the transformation of specified images, and before that it extracts last checkopoint of last run into the protobuf
    python transform.py --inpath=../../data/images/shizu/*.jpg --outdir=../../data/images/shizu/out --includein=0
    python transform.py --inpath=../../dataset-sources/real/images/animefest-2016/*.png --outdir=../../data/images/animefest-2016 --includein=0
    python transform.py --random=30 --outdir=../../data/images/ade20k --includein=1
    python transform.py --inpath=../../dataset-sources/real/images/animefest-2017-cosplay/*.png --outdir=../../data/images/animefest-2017-cosplay --includein=0
    python transform.py --inpath=../../data/images/20180625-1659-0/20000/*-in.png --extract --rundir=20180625-1659-0 extracts last checkpoint from specified run, and then transforms specified images
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

FLAGS = tf.flags.FLAGS

tf.flags.DEFINE_bool('extract', False,
                     'if true, extraction of newest checkpoint will be performed. Needs tensorflow checkpoints')
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


def get_base_name(name):
    return osp.basename(os.path.splitext(name)[0])


def save_image(shapes, images, out_dir, suffix, in_filenames):
    # whole batch of images is here
    shape = shapes[0]  # shapes, because there is shape for each picture in the batch
    y, x = shape / (shape.max() / 512)  # 512 is size of input, which is rectangular
    for i in range(images.shape[0]):  # iterate through the batch
        image = images[i]
        in_filename = in_filenames[i].decode("utf-8")
        size_y, size_x = image.shape[0:2]
        y_from, x_from = int((size_y - y) / 2), int((size_x - x) / 2)
        y_to, x_to = size_y - y_from, size_x - x_from
        cropped_im = image[y_from:y_to, x_from:x_to]
        basename = get_base_name(in_filename)
        new_name = osp.join(out_dir, "{}{}.png".format(basename, suffix))
        imsave(new_name, cropped_im)


def transform_files(im_paths):
    print('will transform {} images: '.format(len(im_paths)))

    def load_image(filename):
        image_string = tf.read_file(filename)
        image = tf.image.decode_png(image_string)
        return image

    def reshape_image(image):
        # image = tf.py_func(lambda x: process_sample(x, True), [image], tf.uint8)
        image = process_sample_tf(image, True)
        image = tf.cast(image, dtype=tf.float32)
        return image

    pb_dir, rundir, step = extract_and_get_pb_dir()
    print('model loaded, starting with inference')

    with tf.device('/cpu:0'):
        orig_names = tf.data.Dataset.from_tensor_slices(im_paths)
        # some images are not valid, this filters them out
        # orig_names = orig_names.filter(lambda x: tf.py_func(is_valid_image, [x], tf.bool))
        orig_images = orig_names.map(load_image, num_parallel_calls=10)
        orig_images = orig_images.apply(tf.contrib.data.ignore_errors())    # this skips invalid images
        orig_shapes = orig_images.map(lambda x: tf.shape(x)[0:2])
        reshaped_images = orig_images.map(reshape_image, num_parallel_calls=10)
        data = tf.data.Dataset.zip((reshaped_images, orig_shapes, orig_names))
        data = data.batch(batch_size=FLAGS.batchsize)
        data = data.prefetch(buffer_size=3)
        iterator = data.make_one_shot_iterator()
        feeder = iterator.get_next()

    out_dir = osp.join(FLAGS.outdir, rundir, step)
    os.makedirs(out_dir, exist_ok=True)

    def persist_images_postprocessing(out_images, in_shapes, in_filenames):
        return tf.py_func(persist_image, [out_images, in_shapes, in_filenames],
                          tf.int32)  # apparently it must return something

    def persist_image(out_image, in_shape, in_filenames):
        if FLAGS.includein:
            save_image(in_shape, out_image, out_dir, '-in', in_filenames)
        save_image(in_shape, out_image, out_dir, '-out', in_filenames)
        return 0

    cycle.CycleGAN.test_one_part(
        osp.join(pb_dir, step, '{}2{}.pb'.format(FLAGS.Xname, FLAGS.Yname)),
        feeder, ceil(len(im_paths) / FLAGS.batchsize), FLAGS.batchsize, persist_images_postprocessing)


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

    transform_files(im_paths)
    print('all done')


def extract_and_get_pb_dir():
    if FLAGS.rundir is None:
        FLAGS.rundir = sorted([d for d in os.listdir(FLAGS.cpdir)
                               if osp.isdir(osp.join(FLAGS.cpdir, d))])[-1]
    fulldir = osp.join(FLAGS.cpdir, FLAGS.rundir)
    print('rundir: ', FLAGS.rundir)
    pb_dir = osp.join(FLAGS.cpdir, '..', 'export', FLAGS.rundir)
    if FLAGS.extract:
        if not osp.exists(pb_dir):
            os.makedirs(pb_dir)
        step = load_and_export(fulldir, pb_dir)
    else:
        import_model()  # model is imported during load_and_export in other case
        step = str(max([int(d) for d in os.listdir(pb_dir) if osp.isdir(osp.join(pb_dir, d))]))
    return pb_dir, FLAGS.rundir, step


if __name__ == '__main__':
    tf.app.run()
