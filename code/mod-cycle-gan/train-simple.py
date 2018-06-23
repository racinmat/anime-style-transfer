#!/usr/bin/env python3

import importlib
import logging
from datetime import datetime

import tensorflow as tf
import cycle
import os.path as osp
import os
from cycle import utils


def _valid_names(module):
    return [mod for mod in dir(module) if not mod.startswith('_')]


NEEDED_IMPORTS = ['X_Discriminator', 'XY_Generator', 'Y_Discriminator', 'YX_Generator',
                  'X_denormer', 'X_normer', 'Y_denormer', 'Y_normer',
                  'X_DATA_SHAPE', 'Y_DATA_SHAPE', 'X_name', 'Y_name']
FLAGS = tf.flags.FLAGS

# Flags for train
tf.flags.DEFINE_string('model', 'lidar',
                       'Model dir to use. Can choose from %s.\nNeeds to export {X,Y}_{normer,denormer} and functions, {X,Y}_DATA_SHAPE and classes {XY,YX}_{Generator,Discriminator}.' % str(
                           _valid_names(cycle.models)))
tf.flags.DEFINE_string('Xtfr', '/datagrid/personal/racinmat/anime-style-transfer/datasets/real/cityscapes.tfrecord',
                       'tfrecords file for X dataset')
tf.flags.DEFINE_string('Ytfr',
                       '/datagrid/personal/racinmat/anime-style-transfer/datasets/anime/no-game-no-life.tfrecord',
                       'tfrecords file for Y dataset')
tf.flags.DEFINE_string('gantype', 'LSGAN',
                       'Type of GAN to use within CycleGAN. Can choose from GAN/LSGAN/WGAN.\nWhile it is theoretically possible to use different GAN type for each part of training, it is generally not advised.')
tf.flags.DEFINE_string('XYgenstr', 'c-7-1-64-r;c-5-2-128-r;b-3-3-r;r-5-1-64-2-r;c-7-1-3-t;sc',
                       'Param string for XY generator')
tf.flags.DEFINE_string('YXgenstr', 'c-7-1-64-r;c-5-2-128-r;b-3-3-r;r-5-1-64-2-r;c-7-1-3-t;sc',
                       'Param string for YX generator')
tf.flags.DEFINE_string('Xdisstr', 'c-5-2-64-l;c-5-2-128-l;c-5-2-256-l;c-5-2-1-s;', 'Param string for X discriminator')
tf.flags.DEFINE_string('Ydisstr', 'c-5-2-64-l;c-5-2-128-l;c-5-2-256-l;c-5-2-1-s;', 'Param string for Y discriminator')
tf.flags.DEFINE_float('XYgll', 2, 'Lambda for XY Generator loss')
tf.flags.DEFINE_float('YXgll', 2, 'Lambda for YX Generator loss')
tf.flags.DEFINE_float('XYsrl', 0, 'Lambda for XY Generator self regularization loss')
tf.flags.DEFINE_float('YXsrl', 0, 'Lambda for YX Generator self regularization loss')
tf.flags.DEFINE_float('Xdll', 2, 'Lambda for X Discriminator loss')
tf.flags.DEFINE_float('Ydll', 2, 'Lambda for Y Discriminator loss')
tf.flags.DEFINE_float('XYgwl', 2, 'Lambda for XY Generator weight loss')
tf.flags.DEFINE_float('YXgwl', 2, 'Lambda for YX Generator weight loss')
tf.flags.DEFINE_float('Xdwl', 2, 'Lambda for X Discriminator weight loss')
tf.flags.DEFINE_float('Ydwl', 2, 'Lambda for Y Discriminator weight loss')
tf.flags.DEFINE_float('cll', 2, 'Cycle loss lambda')
tf.flags.DEFINE_float('Ygrl', 2, 'Lambda for gradient loss for Y discriminator (applicable only if gantype is WGAN)')
tf.flags.DEFINE_float('Xgrl', 2, 'Lambda for gradient loss for X discriminator (applicable only if gantype is WGAN)')
tf.flags.DEFINE_bool('visualize', True,
                     'Whether to visualize during training in tensorboard. model then must have function visualize. Only valid if tbverbose.')
tf.flags.DEFINE_bool('tbverbose', True, 'Whether to export tensorboard data for visualization using Tensorboard.')
tf.flags.DEFINE_bool('verbose', True, 'Verbose mode.')
tf.flags.DEFINE_bool('selftransform', True,
                     'Whether to use specific feature map transform for self regularization loss. If set to True, it will use function feature_map from model, otherwise it will be an identity. Not used if self regularization lossess\' lambdas set to 0.')
tf.flags.DEFINE_integer('dtsteps', 1, 'How many times will be trained discriminator during one step.')
tf.flags.DEFINE_integer('gtsteps', 1, 'How many times will be trained generator during one step.')
tf.flags.DEFINE_integer('batchsize', 1, 'Batch size')
tf.flags.DEFINE_string('norm', 'instance', 'One of two strings instance/batch.')
tf.flags.DEFINE_integer('steps', 200000, 'For how many steps will the model be trained.')
tf.flags.DEFINE_float('decayfrom', 0.5,
                      'Since when (as a percentage of a number of steps) will the learning rate start decaying.')
tf.flags.DEFINE_float('lr', 2e-4, 'Initial learning rate for adam optimizer.')
tf.flags.DEFINE_float('beta1', 0.5, 'Beta for optimizers.')
tf.flags.DEFINE_bool('history', True, 'Whether to keep history of generated images and sometimes swap them.')
tf.flags.DEFINE_integer('poolsize', 50, 'How large a history buffer will be. Valid only if history set to True.')


def main(_):
    logging.getLogger().setLevel(logging.INFO)

    tfr = cycle.utils.TFReader(FLAGS.Xtfr, 'anime', (512, 512, 3))
    feeder = tfr.feed()
    with tf.Session() as sess:
        sess.run(feeder)

    print('sucesfully run pure loading')
    print('Xname:', FLAGS.Xname)
    print('Yname:', FLAGS.Yname)
    # flagstr = '\n'.join(tf.flags.flag_dict_to_args(dict((k, getattr(FLAGS, k))
    #                                                     for k in dir(FLAGS)
    #                                                     if k != 'h' and 'help' not in k and getattr(FLAGS,
    #                                                                                                 k) is not None)))
    modellib = importlib.import_module('cycle.models.' + FLAGS.model)
    modelnames = _valid_names(modellib)
    if FLAGS.visualize:
        NEEDED_IMPORTS.append('visualize')
    if FLAGS.selftransform and (FLAGS.XYsrl > 0 or FLAGS.YXsrl > 0):
        NEEDED_IMPORTS.append('feature_map')
    found = [imp in modelnames for imp in NEEDED_IMPORTS]
    if not all(found):
        raise ImportError('Failed to find some necessary component in your model! '
                          'The components not found are %s'
                          % (str([imp for imp, f in zip(NEEDED_IMPORTS, found) if not f]),))
    if FLAGS.Xname is None:
        FLAGS.Xname = modellib.X_name
    if FLAGS.Yname is None:
        FLAGS.Yname = modellib.Y_name
    xfeed = cycle.utils.TFReader(FLAGS.Xtfr, FLAGS.Xname,
                                 modellib.X_DATA_SHAPE, normer=modellib.X_normer,
                                 denormer=modellib.X_denormer, batch_size=FLAGS.batchsize)
    yfeed = cycle.utils.TFReader(FLAGS.Ytfr, FLAGS.Yname,
                                 modellib.Y_DATA_SHAPE, normer=modellib.Y_normer,
                                 denormer=modellib.Y_denormer, batch_size=FLAGS.batchsize)
    xygen = modellib.XY_Generator(FLAGS.Xname + '-' + FLAGS.Yname + '-gen', FLAGS.XYgenstr,
                                  True, FLAGS.XYgwl, FLAGS.norm)
    yxgen = modellib.YX_Generator(FLAGS.Yname + '-' + FLAGS.Xname + '-gen', FLAGS.YXgenstr,
                                  True, FLAGS.YXgwl, FLAGS.norm)
    xdis = modellib.X_Discriminator(FLAGS.Xname + '-dis', FLAGS.Xdisstr,
                                    True, FLAGS.Xdwl, FLAGS.norm)
    ydis = modellib.Y_Discriminator(FLAGS.Yname + '-dis', FLAGS.Ydisstr,
                                    True, FLAGS.Ydwl, FLAGS.norm)

    xy_fmap = modellib.feature_map if FLAGS.selftransform and FLAGS.XYsrl > 0 else None
    yx_fmap = modellib.feature_map if FLAGS.selftransform and FLAGS.YXsrl > 0 else None

    if FLAGS.gantype.casefold() == 'GAN'.casefold():
        xy = cycle.nets.GAN(xygen, ydis, modellib.X_DATA_SHAPE, modellib.Y_DATA_SHAPE,
                            FLAGS.XYgll, FLAGS.Ydll, FLAGS.XYsrl, xy_fmap)
        yx = cycle.nets.GAN(yxgen, xdis, modellib.Y_DATA_SHAPE, modellib.X_DATA_SHAPE,
                            FLAGS.YXgll, FLAGS.Xdll, FLAGS.YXsrl, yx_fmap)
    elif FLAGS.gantype.casefold() == 'LSGAN'.casefold():
        xy = cycle.nets.LSGAN(xygen, ydis, modellib.X_DATA_SHAPE, modellib.Y_DATA_SHAPE,
                              FLAGS.XYgll, FLAGS.Ydll, FLAGS.XYsrl, xy_fmap)
        yx = cycle.nets.LSGAN(yxgen, xdis, modellib.Y_DATA_SHAPE, modellib.X_DATA_SHAPE,
                              FLAGS.YXgll, FLAGS.Xdll, FLAGS.YXsrl, yx_fmap)
    elif FLAGS.gantype.casefold() == 'WGAN'.casefold():
        xy = cycle.nets.WGAN(xygen, ydis, modellib.X_DATA_SHAPE, modellib.Y_DATA_SHAPE,
                             FLAGS.XYgll, FLAGS.Ydll, FLAGS.Ygrl, FLAGS.XYsrl, xy_fmap)
        yx = cycle.nets.WGAN(yxgen, xdis, modellib.Y_DATA_SHAPE, modellib.X_DATA_SHAPE,
                             FLAGS.YXgll, FLAGS.Xdll, FLAGS.Xgrl, FLAGS.YXsrl, yx_fmap)
    else:
        raise ValueError('You did not specify any gantype that would be recognizible!')
    cygan = cycle.CycleGAN(xy, yx, xfeed, yfeed, FLAGS.Xname, FLAGS.Yname,
                           FLAGS.cll, FLAGS.tbverbose,
                           modellib.visualize if FLAGS.visualize else None,
                           FLAGS.lr, FLAGS.beta1, FLAGS.steps,
                           (FLAGS.decayfrom * FLAGS.steps), FLAGS.history)

    checkpoints_dir = FLAGS.cpdir
    gen_train = FLAGS.gtsteps
    dis_train = FLAGS.dtsteps
    pool_size = FLAGS.poolsize
    load_model = FLAGS.rundir
    log_verbose = FLAGS.verbose
    # param_string = flagstr
    export_final = True

    if load_model is not None:
        full_checkpoints_dir = osp.join(checkpoints_dir, load_model)
    else:
        full_checkpoints_dir = osp.join(checkpoints_dir,
                                        datetime.now().strftime('%Y%m%d-%H%M'))
        i = 0
        checkpoints_dir_try = full_checkpoints_dir + '-' + str(i)
        while osp.exists(checkpoints_dir_try):
            i += 1
            checkpoints_dir_try = full_checkpoints_dir + '-' + str(i)
        full_checkpoints_dir = checkpoints_dir_try
    os.makedirs(full_checkpoints_dir, exist_ok=True)

    # if param_string is not None:
    #     with open(osp.join(full_checkpoints_dir, 'params.flagfile'), 'w') as f:
    #         f.write(param_string)

    with cygan.graph.as_default():
        model_ops = cygan.get_model()
        model_ops['summary'] = tf.summary.merge_all()
        train_writer = tf.summary.FileWriter(full_checkpoints_dir, cygan.graph)
        saver = tf.train.Saver()

    config = tf.ConfigProto()
    config.gpu_options.allow_growth = True
    with tf.Session(graph=cygan.graph, config=config) as sess:
        if load_model is not None:
            checkpoint = tf.train.get_checkpoint_state(full_checkpoints_dir)
            if checkpoint is None:
                step = 0
                sess.run(tf.global_variables_initializer())
            else:
                meta_graph_path = checkpoint.model_checkpoint_path + '.meta'
                restore = tf.train.import_meta_graph(meta_graph_path)
                restore.restore(sess, tf.train.latest_checkpoint(full_checkpoints_dir))
                step = int(osp.basename(meta_graph_path).split('-')[1].split('.')[0])
        else:
            step = 0
            sess.run(tf.global_variables_initializer())

        coord = tf.train.Coordinator()
        threads = tf.train.start_queue_runners(sess=sess, coord=coord)

        if log_verbose:
            varsize_dict = sess.run({
                '{}->{} gen - num params'.format(cygan.X_name, cygan.Y_name): cygan.XtoY.gen.num_params(),
                '{}->{} gen - num params'.format(cygan.Y_name, cygan.X_name): cygan.YtoX.gen.num_params(),
                '{} dis - num params'.format(cygan.X_name): cygan.YtoX.dis.num_params(),
                '{} dis - num params'.format(cygan.Y_name): cygan.YtoX.dis.num_params(),
            })
            for k, v in varsize_dict.items():
                logging.info('\t{}:\t{}'.format(k, v))

        if cygan.history:
            x_pool = utils.DataBuffer(pool_size, cygan.X_feed.batch_size)
            y_pool = utils.DataBuffer(pool_size, cygan.Y_feed.batch_size)

        try:
            if cygan.history:
                cur_x, cur_y = sess.run([cygan.X_feed.feed(), cygan.Y_feed.feed()])
            while not coord.should_stop():
                if cygan.history:
                    fx, fy = sess.run(model_ops['fakes'], feed_dict={
                        cygan.cur_x: cur_x,
                        cygan.cur_y: cur_y,
                    })
                cur_x, cur_y = sess.run([cygan.X_feed.feed(), cygan.Y_feed.feed()])
                if cygan.history:
                    feeder_dict = {
                        cygan.cur_x: cur_x,
                        cygan.cur_y: cur_y,
                        cygan.prev_fake_x: x_pool.query(fx, step),
                        cygan.prev_fake_y: y_pool.query(fy, step),
                    }
                else:
                    feeder_dict = {
                        cygan.cur_x: cur_x,
                        cygan.cur_y: cur_y,
                    }
                for _ in range(dis_train):
                    sess.run(model_ops['train']['dis'], feed_dict=feeder_dict)
                for _ in range(gen_train):
                    sess.run(model_ops['train']['gen'], feed_dict=feeder_dict)
                summary, _ = sess.run([model_ops['summary'], model_ops['train']['global_step']],
                                      feed_dict=feeder_dict)

                train_writer.add_summary(summary, step)
                train_writer.flush()

                if log_verbose and step % 100 == 0:
                    logging.info('------ Step {:d} ------'.format(step))
                    losses = sess.run(model_ops['losses'], feed_dict=feeder_dict)
                    for k, v in losses.items():
                        logging.info('\t{}:\t{:8f}'.format(k, v))

                if step % 10000 == 0:
                    save_path = saver.save(sess,
                                           osp.join(full_checkpoints_dir, 'model.ckpt'),
                                           global_step=step)
                    logging.info('Model saved in file: %s', save_path)

                step += 1

                if step >= cygan.steps:
                    logging.info('Stopping after %d iterations', cygan.steps)
                    coord.request_stop()
        except Exception as e:
            if log_verbose:
                logging.info('Interrupted')
                logging.info(e)
            coord.request_stop()
        finally:
            save_path = saver.save(sess,
                                   osp.join(full_checkpoints_dir, 'model.ckpt'),
                                   global_step=step)
            if log_verbose:
                logging.info('Model saved in file: %s', save_path)
            if export_final:
                cygan.export(sess, full_checkpoints_dir)
            coord.request_stop()
            coord.join(threads)


if __name__ == '__main__':
    tf.app.run()
