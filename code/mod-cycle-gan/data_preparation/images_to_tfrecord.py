import logging
import os
import os.path as osp
import glob
import queue
import threading
from io import BytesIO

import progressbar
import tensorflow as tf
import numpy as np
from PIL import Image
from joblib import Parallel, delayed
from object_detection.utils import dataset_util
from common_params import IMAGE_HEIGHT, IMAGE_WIDTH, IMAGES_SIZE, IMAGES_SHAPE

FLAGS = tf.flags.FLAGS

tf.flags.DEFINE_enum('type', None, ['real', 'anime'], 'Type, either anime or real')
tf.flags.DEFINE_string('name', None,
                       'Name of the dataset in tfrecord file (must correspond with Xname and Yname files).')

# images will be resized so they are smaller or equal this resolutions, allows having dynamic shape, but not so big max size
MAX_HEIGHT = 600
MAX_WIDTH = 800


def process_sample_tf(image, padding=False):
    if padding:
        return tf.image.resize_image_with_pad(image, IMAGE_HEIGHT, IMAGE_WIDTH)
    if IMAGES_SIZE == (None, None):
        # dynamic shape, no reshaping
        return image
    else:
        return tf.image.resize_images(image, IMAGES_SIZE, preserve_aspect_ratio=True)


q = None


def image_to_example(f):
    data = np.array(Image.open(f))
    if data.ndim == 2 or data.shape[2] == 1:  # grayscale
        # black and white image detected, skipping
        return
    im = Image.fromarray(data)  # resizing is done in tensorflow during preprocessing, don't modify it here
    if MAX_WIDTH is not None and MAX_HEIGHT is not None:
        im.thumbnail((MAX_WIDTH, MAX_HEIGHT), Image.ANTIALIAS)
    stream = BytesIO()
    im.save(stream, format='PNG')
    # im.save('image-{}.png'.format(i), format='PNG')
    encoded_png = stream.getvalue()
    ex = tf.train.Example(features=tf.train.Features(feature={
        'image/encoded': dataset_util.bytes_feature(encoded_png),
        'image/format': dataset_util.bytes_feature('png'.encode('utf8')),
        'image/source_id': dataset_util.bytes_feature(f.encode('utf8')),
    }))
    serialized = ex.SerializeToString()
    q.put(serialized)


def image_to_tfrecord(i, f, writer, pbar):
    # from time import time
    try:
        pbar.update(i)
        # start = time()
        data = np.array(Image.open(f))
        if data.ndim == 2 or data.shape[2] == 1:  # grayscale
            # black and white image detected, skipping
            return
        im = Image.fromarray(data)  # resizing is done in tensorflow during preprocessing, don't modify it here
        stream = BytesIO()
        im.save(stream, format='PNG')
        # im.save('image-{}.png'.format(i), format='PNG')
        encoded_png = stream.getvalue()
        ex = tf.train.Example(features=tf.train.Features(feature={
            'image/encoded': dataset_util.bytes_feature(encoded_png),
            'image/format': dataset_util.bytes_feature('png'.encode('utf8')),
            'image/source_id': dataset_util.bytes_feature(f.encode('utf8')),
        }))
        serialized = ex.SerializeToString()
        # print(time() - start, 'for data preparation')
        # start = time()
        writer.write(serialized)
        # print(time() - start, 'for writing to file')
    except (IOError, ValueError) as e:
        logging.warning('Opening %s failed', f)
        logging.warning(e)


def run(infiles, outfile):
    outfile = osp.abspath(outfile)
    if osp.isfile(outfile):
        # logging.warning('File %s already exists, it will be overwritten', outfile)
        logging.warning('File %s already exists, skipping', outfile)
        return
    os.makedirs(osp.dirname(outfile), exist_ok=True)
    writer = tf.python_io.TFRecordWriter(outfile)

    widgets = [progressbar.Percentage(), ' ', progressbar.Counter(), ' ', progressbar.Bar(), ' ',
               progressbar.FileTransferSpeed()]
    pbar = progressbar.ProgressBar(widgets=widgets, max_value=len(infiles)).start()

    # TFRecordWriter is not threadsafe, so I have to write to it in single thread,
    # but I can read and prepare data (which takes most of time) in multiple threads

    # for i, f in enumerate(infiles):
    #     image_to_tfrecord(i, f, writer, pbar)

    def images_to_examples():
        workers = 12
        Parallel(n_jobs=workers, backend='threading')(delayed(image_to_example)(f) for f in infiles)
    threading.Thread(target=images_to_examples, name='socket_server').start()

    i = 0
    while i < len(infiles):
        pbar.update(i)
        serialized = q.get()
        writer.write(serialized)
        i += 1

    writer.close()


def run_anime():
    images_root = '../../../dataset-sources/anime/images'
    tfrecords_root = '../../../datasets/anime'
    anime_name = FLAGS.name
    anime_root = os.path.join(images_root, anime_name)
    print('processing directory {}'.format(anime_root))
    run(glob.glob(osp.join(anime_root, '*.png')), osp.join(tfrecords_root, anime_name + '.tfrecord'))


def get_real_images_cityscapes():
    images_root = '/datagrid/public_datasets/CityScapes/leftImg8bit/train'
    infiles = []
    print('processing cityscapes')
    for dir_name in os.listdir(images_root):
        infiles += glob.glob(osp.join(images_root, dir_name, '*.png'))
    return infiles


def get_real_images_ade20k():
    images_root = '../../../dataset-sources/real/images/ADE20K_2016_07_26/images/training'
    print('processing ade20k')
    # ade20k dataset has 1 to 2 dir levels and all ground truth images are jpg ending with number
    infiles = list(glob.glob(osp.join(images_root, '**', '*[0-9].jpg'), recursive=True))
    return infiles


def run_real():
    tfrecords_root = '../../../datasets/real'
    infiles = []
    tfrecord_name = ''
    # infiles = get_real_images_cityscapes()
    # tfrecord_name = 'cityscapes.tfrecord'
    if FLAGS.name == 'ade20k':
        infiles = get_real_images_ade20k()  # hopefully ade20k will be more representative than cityscapes
        tfrecord_name = 'ade20k.tfrecord'
    else:
        images_root = '../../../dataset-sources/real/images'
        dir_name = FLAGS.name
        real_root = os.path.join(images_root, dir_name)
        infiles = list(glob.glob(osp.join(real_root, '**', '*.jpg'), recursive=True))
        infiles += list(glob.glob(osp.join(real_root, '**', '*.png'), recursive=True))
        tfrecord_name = dir_name + '.tfrecord'
    print('{} files to process'.format(len(infiles)))
    run(infiles, osp.join(tfrecords_root, tfrecord_name))


if __name__ == '__main__':
    from time import time

    start = time()

    q = queue.Queue()
    logging.getLogger().setLevel(logging.INFO)
    if FLAGS.type == 'real':
        run_real()
    if FLAGS.type == 'anime':
        run_anime()

    print(time() - start, 'for everything')
