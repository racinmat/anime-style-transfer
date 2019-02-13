#!/usr/bin/env python3

import importlib
import logging
import os
import os.path as osp
import tensorflow as tf
from gan.cifar.util import get_frechet_inception_distance

import cycle
from common_params import IMAGE_HEIGHT, IMAGE_WIDTH
import train
from cycle import CycleGAN
from cycle.models.anime._utils import normer

FLAGS = tf.flags.FLAGS


def get_base_name(name):
    return osp.basename(os.path.splitext(name)[0])


def main(_):
    logging.getLogger().setLevel(logging.INFO)
    FLAGS.batchsize = 16

    pb_dir = osp.join(FLAGS.cpdir, '..', 'export', FLAGS.rundir)

    modellib = train.import_model()
    # reshaping to static size so I can batch it

    def norm_and_resize(data):
        data = normer(data)
        data = tf.image.resize_image_with_crop_or_pad(data, 480, 480)
        return data

    modellib.X_normer = norm_and_resize
    modellib.Y_normer = norm_and_resize

    x_feed, y_feed = train.initialize_readers(modellib, infinite=False)

    step = str(max([int(d) for d in os.listdir(pb_dir) if osp.isdir(osp.join(pb_dir, d))]))

    graph = tf.get_default_graph()
    pb_model_x_to_y = osp.join(pb_dir, step, '{}2{}.pb'.format(FLAGS.Xname, FLAGS.Yname))
    pb_model_y_to_x = osp.join(pb_dir, step, '{}2{}.pb'.format(FLAGS.Yname, FLAGS.Xname))
    _, _, output_y = CycleGAN.get_graph_outputs(graph, x_feed.feed(), pb_model_x_to_y)
    _, _, output_x = CycleGAN.get_graph_outputs(graph, y_feed.feed(), pb_model_y_to_x)

    num_inception_images = train.FLAGS.batchsize
    fid_y = get_frechet_inception_distance(
        real_images=y_feed.feed(), generated_images=output_y, batch_size=train.FLAGS.batchsize,
        num_inception_images=num_inception_images)
    fid_x = get_frechet_inception_distance(
        real_images=x_feed.feed(), generated_images=output_x, batch_size=train.FLAGS.batchsize,
        num_inception_images=num_inception_images)

    config = tf.ConfigProto()
    config.gpu_options.allow_growth = True
    with tf.Session(graph=graph, config=config) as sess:
        sess.run(tf.global_variables_initializer())

        # widgets = [progressbar.Percentage(), ' ', progressbar.Counter(), ' ', progressbar.Bar(), ' ',
        #            progressbar.FileTransferSpeed()]
        # pbar = progressbar.ProgressBar(widgets=widgets, max_value=data_size * batch_size).start()
        i = 0
        while True:
            try:
                # pbar.update(i * batch_size)
                # i += 1
                fid_x_val, fid_y_val, = sess.run([fid_x, fid_y])
                print('fid_x_val:', fid_x_val, 'fid_y_val:', fid_y_val)
            except tf.errors.OutOfRangeError:
                break
    print('done')


if __name__ == '__main__':
    tf.app.run()
