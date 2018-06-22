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
    images_root = '../../dataset-sources/anime/images'
    tfrecords_root = '../../datasets/anime'
    for dir_name in os.listdir(images_root):
        print('processing directory {}'.format(dir_name))
        run(glob.glob(osp.join(images_root, dir_name, '*.png')), osp.join(tfrecords_root, dir_name+'.tfrecord'), 'anime')


def run_real():
    # cityscapes
    images_root = '/datagrid/public_datasets/CityScapes/leftImg8bit/train'
    tfrecords_root = '../../datasets/real'
    infiles = []
    print('processing cityscapes')
    for dir_name in os.listdir(images_root):
        infiles += glob.glob(osp.join(images_root, dir_name, '*.png'))
    run(infiles, osp.join(tfrecords_root, 'cityscapes.tfrecord'), 'real')


if __name__ == '__main__':
    logging.getLogger().setLevel(logging.INFO)
    run_anime()
    run_real()
