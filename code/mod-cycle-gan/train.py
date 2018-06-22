#!/usr/bin/env python3

import importlib
import logging
import tensorflow as tf
import cycle


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
tf.flags.DEFINE_string('Xtfr',
                       '/datagrid/personal/racinmat/anime-style-transfer/datasets/anime/no-game-no-life.tfrecord',
                       'tfrecords file for X dataset')
tf.flags.DEFINE_string('Ytfr', '/datagrid/personal/racinmat/anime-style-transfer/datasets/real/cityscapes.tfrecord',
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
    flagstr = '\n'.join(tf.flags.flag_dict_to_args(dict((k, getattr(FLAGS, k))
                                                        for k in dir(FLAGS)
                                                        if k != 'h' and 'help' not in k and getattr(FLAGS,
                                                                                                    k) is not None)))
    try:
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
    except ImportError:
        logging.warning('Could not import your model %s! Will not continue!', FLAGS.model)
        raise
    else:
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

        cygan.train(FLAGS.cpdir, FLAGS.gtsteps, FLAGS.dtsteps,
                    FLAGS.poolsize, FLAGS.rundir, FLAGS.verbose, flagstr)


if __name__ == '__main__':
    tf.app.run()
