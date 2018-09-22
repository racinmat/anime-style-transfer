import logging
import os
import os.path as osp
import glob
from io import BytesIO

import progressbar
import tensorflow as tf
import numpy as np
from PIL import Image
from joblib import Parallel, delayed
from object_detection.utils import dataset_util
from scipy.misc import imresize


FLAGS = tf.flags.FLAGS

tf.flags.DEFINE_enum('type', None, ['real', 'anime'], 'Type, either anime or real')
tf.flags.DEFINE_string('name', None, 'Name of the dataset in tfrecord file (must correspond with Xname and Yname files).')


def make_square(im, fill_color=(0, 0, 0, 0)):
    y, x = im.shape[0:2]
    size = max(x, y)
    new_im = Image.new('RGB', (size, size), fill_color)
    new_im.paste(Image.fromarray(im), (int((size - x) / 2), int((size - y) / 2)))
    return np.array(new_im)


def process_sample(data, padding=False):
    if padding:
        data = make_square(data)
    return imresize(data, (512, 512))


def image_to_tfrecord(i, f, writer, pbar):
    try:
        pbar.update(i)
        data = np.array(Image.open(f))
        if data.ndim == 2 or data.shape[2] == 1:  # grayscale
            # black and white image detected, skipping
            return
        im = Image.fromarray(data)  # resizing is done in tensorflow during preprocessing, don't modify it here
        stream = BytesIO()
        im.save(stream, format='PNG')
        encoded_png = stream.getvalue()
        ex = tf.train.Example(features=tf.train.Features(feature={
            'image/encoded': dataset_util.bytes_feature(encoded_png),
            'image/format': dataset_util.bytes_feature('png'.encode('utf8')),
            'image/source_id': dataset_util.bytes_feature(f.encode('utf8')),
        }))
        writer.write(ex.SerializeToString())
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

    workers = 12
    Parallel(n_jobs=workers, backend='threading')(
        delayed(image_to_tfrecord)(i, f, writer, pbar) for i, f in enumerate(infiles))

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
    print('{} files to process'.format(len(infiles)))
    run(infiles, osp.join(tfrecords_root, tfrecord_name))


if __name__ == '__main__':
    logging.getLogger().setLevel(logging.INFO)
    if FLAGS.type == 'real':
        run_real()
    if FLAGS.type == 'anime':
        run_anime()
