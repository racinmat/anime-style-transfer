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
    shapes = np.zeros((len(im_paths), 2), dtype=np.int32)       # orig shape so padded data can be cropped later

    # show progressbar for more images
    for i, f in enumerate(im_paths):
        try:
            orig_image = np.array(Image.open(f))
            shapes[i, :] = orig_image.shape[0:2]
            image = process_sample(orig_image, True)
            data[i, :, :, :] = image
        except OSError:
            data[i, :, :, :] = data[i-1, :, :, :]   # just use previous sample if anything goes wrong

    return data, shapes


def one_image_to_numpy(i, f, pbar, data, shapes, counter):
    pbar.update(counter[0])
    counter[0] += 1
    try:
        orig_image = np.array(Image.open(f))
        shapes[i, :] = orig_image.shape[0:2]
        image = process_sample(orig_image, True)
        data[i, :, :, :] = image
    except OSError:
        data[i, :, :, :] = data[i - 1, :, :, :]  # just use previous sample if anything goes wrong


def images_to_numpy_parallel(im_paths):
    one_img_size = X_DATA_SHAPE
    data = np.zeros((len(im_paths), one_img_size[0], one_img_size[1], one_img_size[2]), dtype=np.float32)
    shapes = np.zeros((len(im_paths), 2), dtype=np.int32)       # orig shape so padded data can be cropped later

    widgets = [progressbar.Percentage(), ' ', progressbar.Counter(), ' ', progressbar.Bar(), ' ',
               progressbar.FileTransferSpeed()]
    pbar = progressbar.ProgressBar(widgets=widgets, max_value=len(im_paths)).start()

    counter = [0]
    workers = 10

    Parallel(n_jobs=workers, backend='threading')(
        delayed(one_image_to_numpy)(i, f, pbar, data, shapes, counter) for i, f in enumerate(im_paths))

    pbar.finish()

    return data, shapes


def numpy_to_images(data, shapes, out_dir, suffix='-out'):
    # todo: fixnout, pro více volání za sebou přemazává stejně pojmenované soubory, asi předělat na dataset api?
    # todo: propojit s yield a shape z dataset api
    # todo: předělat loading na nový tfrecord formát
    os.makedirs(out_dir, exist_ok=True)
    print('going to persist {} images'.format(len(data)))
    num_digits = int(np.log10(len(data)))+1

    widgets = [progressbar.Percentage(), ' ', progressbar.Counter(), ' ', progressbar.Bar(), ' ',
               progressbar.FileTransferSpeed()]
    pbar = progressbar.ProgressBar(widgets=widgets, max_value=len(data)).start()

    for i, im in enumerate(data):
        pbar.update(i)
        save_image(shapes[i, :], im, out_dir, suffix, num_digits, i)
    pbar.finish()


def save_image(shape, image, out_dir, suffix, num_digits, i):
    y, x = shape / (shape.max() / 512)  # 512 is size of input, which is rectangular
    size_y, size_x = image.shape[0:2]
    y_from, x_from = int((size_y - y) / 2), int((size_x - x) / 2)
    y_to, x_to = size_y - y_from, size_x - x_from
    cropped_im = image[y_from:y_to, x_from:x_to]
    imsave(osp.join(out_dir, '{}{}.png'.format(str(i).zfill(num_digits), suffix)), cropped_im)


def transform_files(im_paths, eager_load=True):
    print('will transform {} images: '.format(len(im_paths)))

    def _parse_example(filename):
        image_string = tf.read_file(filename)
        image = tf.image.decode_png(image_string)
        im_shape = image.shape[0:2] # shape not workling, todo: read shape from tensorflow
        image = tf.py_func(lambda x: process_sample(x, True), [image], tf.uint8)
        image = tf.cast(image, dtype=tf.float32)
        return tf.group(image, im_shape)
    # todo: předělat do nového tfrecord formátu, používat jeden název fíčur v tfrecordu

    pb_dir, rundir, step = extract_and_get_pb_dir()
    print('model loaded, starting with inference')

    # keeping original sizes before reshaping so I can crop black stripes from reshaped images
    if eager_load:
        in_data, in_shapes = images_to_numpy(im_paths)
        print('images prepared in numpy')
        outputs = cycle.CycleGAN.test_one_part(
            osp.join(pb_dir, step, '{}2{}.pb'.format(FLAGS.Xname, FLAGS.Yname)),
            in_data)

        print('data transformed, going to persist them')

        if FLAGS.includein:
            numpy_to_images(in_data, in_shapes, osp.join(FLAGS.outdir, rundir, step), suffix='-in')
        numpy_to_images(outputs, in_shapes, osp.join(FLAGS.outdir, rundir, step), suffix='-out')
    else:
        data = tf.data.Dataset.from_tensor_slices(im_paths)
        data = data.map(_parse_example, num_parallel_calls=10)
        data = data.batch(batch_size=8)
        iterator = data.make_one_shot_iterator()
        feeder = iterator.get_next()

        out_dir = osp.join(FLAGS.outdir, rundir, step)
        num_digits = int(np.log10(len(im_paths))) + 1
        os.makedirs(out_dir, exist_ok=True)

        def persist_images_postprocessing(out_images, in_shapes, iteration):
            return tf.py_func(persist_image, [out_images, in_shapes, iteration])

        def persist_image(out_image, in_shape, i):
            if FLAGS.includein:
                save_image(in_shape, out_image, out_dir, '-in', num_digits, i)
            save_image(in_shape, out_image, out_dir, '-out', num_digits, i)

        cycle.CycleGAN.test_one_part_dataset(
            osp.join(pb_dir, step, '{}2{}.pb'.format(FLAGS.Xname, FLAGS.Yname)),
            feeder, len(im_paths), persist_images_postprocessing)


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

    chunk_size = 1000
    if len(im_paths) <= chunk_size:
        # everything fits into memory at once without problem
        transform_files(im_paths, eager_load=True)
    else:
        transform_files(im_paths, eager_load=False)
    print('all done')


def extract_and_get_pb_dir():
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
        import_model()  # model is imported during load_and_export in other case
        pb_dir = osp.join(FLAGS.cpdir, '..', 'export')
        rundir = sorted([d for d in os.listdir(pb_dir) if osp.isdir(osp.join(pb_dir, d))])[-1]
        full_rundir = osp.join(pb_dir, rundir)
        print('rundir:', rundir)
        step = str(max([int(d) for d in os.listdir(full_rundir) if osp.isdir(osp.join(full_rundir, d))]))
        pb_dir = osp.join(pb_dir, rundir)
    return pb_dir, rundir, step


if __name__ == '__main__':
    tf.app.run()
