import logging
import os
import os.path as osp
import glob

import progressbar
import tensorflow as tf
import numpy as np
from PIL import Image
from scipy.misc import imresize


def process_sample(data):
    return imresize(data, (512, 512))


def run(infiles, outfile, tf_name):
    outfile = osp.abspath(outfile)
    if osp.isfile(outfile):
        # logging.warning('File %s already exists, it will be overwritten', outfile)
        logging.warning('File %s already exists, skipping', outfile)
        return
    os.makedirs(osp.dirname(outfile), exist_ok=True)
    writer = tf.python_io.TFRecordWriter(outfile)
    name = tf_name + '_data'

    widgets = [progressbar.Percentage(), ' ', progressbar.Counter(), ' ', progressbar.Bar(), ' ',
               progressbar.FileTransferSpeed()]
    pbar = progressbar.ProgressBar(widgets=widgets, max_value=len(infiles)).start()

    for i, f in enumerate(infiles):
        pbar.update(i)
        try:
            data = np.array(Image.open(f))
            # if i % 10 == 0:
            #     logging.info('Opening %s', f)
            #     logging.info('%d / %d : \t %s', i, len(infiles), f)
            ex = tf.train.Example(features=tf.train.Features(
                feature={name: tf.train.Feature(float_list=tf.train.FloatList(value=process_sample(data).ravel()))}))
            writer.write(ex.SerializeToString())
        except (IOError, ValueError) as e:
            logging.warning('Opening %s failed', f)
            logging.warning(e)
    writer.close()


def run_anime():
    images_root = '../../../dataset-sources/anime/images'
    tfrecords_root = '../../../datasets/anime'
    for dir_name in os.listdir(images_root):
        print('processing directory {}'.format(dir_name))
        run(glob.glob(osp.join(images_root, dir_name, '*.png')), osp.join(tfrecords_root, dir_name + '.tfrecord'),
            'anime')


def get_real_images_cityscapes():
    images_root = '/datagrid/public_datasets/CityScapes/leftImg8bit/train'
    infiles = []
    print('processing cityscapes')
    for dir_name in os.listdir(images_root):
        infiles += glob.glob(osp.join(images_root, dir_name, '*.png'))
    return infiles


def get_real_images_ade20k():
    images_root = '/datagrid/public_datasets/ADE20K_2016_07_26/images/training'
    print('processing ade20k')
    # ade20k dataset has 1 to 2 dir levels and all ground truth images are jpg ending with number
    infiles = list(glob.glob(osp.join(images_root, '**', '*[0-9].jpg'), recursive=True))
    return infiles


def run_real():
    tfrecords_root = '../../../datasets/real'
    # infiles = get_real_images_cityscapes()
    # tfrecord_name = 'cityscapes.tfrecord'
    infiles = get_real_images_ade20k()  # hopefully ade20k will be more representative than cityscapes
    tfrecord_name = 'ade20k.tfrecord'
    print('{} files to process'.format(len(infiles)))
    run(infiles, osp.join(tfrecords_root, tfrecord_name), 'real')


if __name__ == '__main__':
    logging.getLogger().setLevel(logging.INFO)
    # run_anime()
    run_real()
