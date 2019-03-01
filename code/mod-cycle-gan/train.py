#!/usr/bin/env python3

import importlib
import logging
import os

import tensorflow as tf
import cycle
from common_params import IMAGE_HEIGHT, IMAGE_WIDTH


def _valid_names(module):
    return [mod for mod in dir(module) if not mod.startswith('_')]


NEEDED_IMPORTS = ['X_Discriminator', 'XY_Generator', 'Y_Discriminator', 'YX_Generator',
                  'X_denormer', 'X_normer', 'Y_denormer', 'Y_normer',
                  'X_DATA_SHAPE', 'Y_DATA_SHAPE', 'X_name', 'Y_name']
FLAGS = tf.flags.FLAGS

# Flags for train
tf.flags.DEFINE_string('model', 'anime',
                       'Model dir to use. Can choose from %s.\nNeeds to export {X,Y}_{normer,denormer} and functions, '
                       '{X,Y}_DATA_SHAPE and classes {XY,YX}_{Generator,Discriminator}.' % str(
                           _valid_names(cycle.models)))
tf.flags.DEFINE_list('Xtfr', '../../datasets/real/ade20k.tfrecord',
                     'tfrecords files for X dataset (can be 1 or more files)')
tf.flags.DEFINE_list('Ytfr', '../../datasets/anime/no-game-no-life.tfrecord',
                     'tfrecords files for Y dataset (can be 1 or more files)')
tf.flags.DEFINE_enum('gantype', 'LSGAN', ['GAN', 'LSGAN', 'WGAN'],
                     'Type of GAN to use within CycleGAN. Can choose from GAN/LSGAN/WGAN.\n'
                     'While it is theoretically possible to use different GAN type for each part of training, '
                     'it is generally not advised.')
tf.flags.DEFINE_string('XYgenstr', 'c-7-1-64-r;c-5-2-128-r;b-3-3-r;r-5-1-64-2-r;c-7-1-3-t;sc',
                       'Param string for XY generator')
tf.flags.DEFINE_string('YXgenstr', 'c-7-1-64-r;c-5-2-128-r;b-3-3-r;r-5-1-64-2-r;c-7-1-3-t;sc',
                       'Param string for YX generator')
tf.flags.DEFINE_string('Xdisstr', 'c-5-2-64-l;c-5-2-128-l;c-5-2-256-l;c-5-2-1-i;', 'Param string for X discriminator')
tf.flags.DEFINE_string('Ydisstr', 'c-5-2-64-l;c-5-2-128-l;c-5-2-256-l;c-5-2-1-i;', 'Param string for Y discriminator')

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
tf.flags.DEFINE_float('cll_end', 2, 'Cycle loss lambda end '
                                    '(if different from cll, linear lambda decay will be performed)')
tf.flags.DEFINE_float('Ygrl', 2, 'Lambda for gradient loss for Y discriminator (applicable only if gantype is WGAN)')
tf.flags.DEFINE_float('Xgrl', 2, 'Lambda for gradient loss for X discriminator (applicable only if gantype is WGAN)')
tf.flags.DEFINE_bool('Ygr_os', False, 'If penalty on Y discriminator will be onesided or not '
                                      '(applicable only if gantype is WGAN)')
tf.flags.DEFINE_bool('Xgr_os', False, 'If penalty on Y discriminator will be onesided or not '
                                      '(applicable only if gantype is WGAN)')

tf.flags.DEFINE_bool('visualize', True,
                     'Whether to visualize during training in tensorboard. model then must have function visualize. '
                     'Only valid if tbverbose.')
tf.flags.DEFINE_bool('tbverbose', True, 'Whether to export tensorboard data for visualization using Tensorboard.')
tf.flags.DEFINE_bool('verbose', True, 'Verbose mode.')
tf.flags.DEFINE_bool('selftransform', False,
                     'Whether to use specific feature map transform for self regularization loss. If set to True, '
                     'it will use function feature_map from model, otherwise it will be an identity. '
                     'Not used if self regularization lossess\' lambdas set to 0.')
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
tf.flags.DEFINE_float('ls', 0.0, 'Label smoothing - Which portion will be assigned to other labels.'
                                 'Only one-sided version supported (no reason to smooth fake labels).')


def keep_flag(k): return k != 'h' and 'help' not in k and getattr(FLAGS, k) is not None


def main(_):
    logger = logging.getLogger()
    logger.setLevel(logging.INFO)
    logging.basicConfig()
    hdlr = logger.handlers[0]
    formatter = logging.Formatter("%(asctime)s:%(levelname)s:%(message)s", "%Y-%m-%d %H:%M")
    hdlr.setFormatter(formatter)

    flagstr = '\n'.join(tf.flags.flag_dict_to_args(dict((k, getattr(FLAGS, k)) for k in dir(FLAGS) if keep_flag(k))))

    assert FLAGS.batchsize == 1 or (
            IMAGE_HEIGHT is not None and IMAGE_WIDTH is not None), 'either use batch size 1 or specify input shapes'
    # I have only one GPU, no need for this now
    # # setting up only one GPU to use
    # devices_environ_var = 'CUDA_VISIBLE_DEVICES'
    # if devices_environ_var not in os.environ:
    #     raise Exception(devices_environ_var + ' variable not found in environment')
    # available_devices = os.environ[devices_environ_var].split(',')
    # if not len(available_devices):
    #     raise Exception(devices_environ_var + 'no gpu visible')
    # gpu = FLAGS.gpu_idx
    # os.environ[devices_environ_var] = available_devices[gpu]

    cygan = create_cyclegan()

    cygan.train(FLAGS.gtsteps, FLAGS.dtsteps, FLAGS.verbose, flagstr)


def create_cyclegan():
    modellib, xfeed, xy, yfeed, yx = initialize_networks()
    visualize = modellib.visualize if FLAGS.visualize else None
    decay_start = int(FLAGS.decayfrom * FLAGS.steps)
    if FLAGS.history:
        cygan = cycle.HistoryCycleGAN(
            xy, yx, xfeed, yfeed, FLAGS.Xname, FLAGS.Yname, FLAGS.cll, FLAGS.cll_end, FLAGS.tbverbose, visualize,
            FLAGS.lr, FLAGS.beta1, FLAGS.steps, decay_start, checkpoints_dir=FLAGS.cpdir, load_model=FLAGS.rundir,
            pool_size=FLAGS.poolsize)
    else:
        cygan = cycle.CycleGAN(
            xy, yx, xfeed, yfeed, FLAGS.Xname, FLAGS.Yname, FLAGS.cll, FLAGS.cll_end, FLAGS.tbverbose, visualize,
            FLAGS.lr, FLAGS.beta1, FLAGS.steps, decay_start, checkpoints_dir=FLAGS.cpdir, load_model=FLAGS.rundir)
    return cygan


def import_model():
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
    return modellib


def initialize_networks():
    modellib = import_model()
    xfeed, yfeed = initialize_readers(modellib)
    xygen = modellib.XY_Generator(FLAGS.Xname + '-' + FLAGS.Yname + '-gen', FLAGS.XYgenstr, True, FLAGS.XYgwl,
                                  FLAGS.norm)
    yxgen = modellib.YX_Generator(FLAGS.Yname + '-' + FLAGS.Xname + '-gen', FLAGS.YXgenstr, True, FLAGS.YXgwl,
                                  FLAGS.norm)
    xdis = modellib.X_Discriminator(FLAGS.Xname + '-dis', FLAGS.Xdisstr, True, FLAGS.Xdwl, FLAGS.norm)
    ydis = modellib.Y_Discriminator(FLAGS.Yname + '-dis', FLAGS.Ydisstr, True, FLAGS.Ydwl, FLAGS.norm)
    xy_fmap = modellib.feature_map if FLAGS.selftransform and FLAGS.XYsrl > 0 else None
    yx_fmap = modellib.feature_map if FLAGS.selftransform and FLAGS.YXsrl > 0 else None

    YXgll = FLAGS.YXgll
    XYgll = FLAGS.XYgll
    Xdll = FLAGS.Xdll
    Ydll = FLAGS.Ydll
    Ygrl = FLAGS.Ygrl
    Xgrl = FLAGS.Xgrl
    XYsrl = FLAGS.XYsrl
    YXsrl = FLAGS.YXsrl
    X_shape = modellib.X_DATA_SHAPE
    Y_shape = modellib.Y_DATA_SHAPE
    tbverbose = FLAGS.tbverbose
    ls = FLAGS.ls

    if FLAGS.gantype.casefold() == 'GAN'.casefold():
        xy = cycle.nets.GAN(xygen, ydis, X_shape, Y_shape, tbverbose, XYgll, Ydll, XYsrl, ls, xy_fmap)
        yx = cycle.nets.GAN(yxgen, xdis, Y_shape, X_shape, tbverbose, YXgll, Xdll, YXsrl, ls, yx_fmap)
    elif FLAGS.gantype.casefold() == 'LSGAN'.casefold():
        xy = cycle.nets.LSGAN(xygen, ydis, X_shape, Y_shape, tbverbose, XYgll, Ydll, XYsrl, ls, xy_fmap)
        yx = cycle.nets.LSGAN(yxgen, xdis, Y_shape, X_shape, tbverbose, YXgll, Xdll, YXsrl, ls, yx_fmap)
    elif FLAGS.gantype.casefold() == 'WGAN'.casefold():
        xy = cycle.nets.WGAN(xygen, ydis, X_shape, Y_shape, tbverbose, XYgll, Ydll, Ygrl, XYsrl, ls, xy_fmap,
                             FLAGS.Ygr_os)
        yx = cycle.nets.WGAN(yxgen, xdis, Y_shape, X_shape, tbverbose, YXgll, Xdll, Xgrl, YXsrl, ls, yx_fmap,
                             FLAGS.Xgr_os)
    else:
        raise ValueError('You did not specify any gantype that would be recognizible!')
    return modellib, xfeed, xy, yfeed, yx


def initialize_readers(modellib, infinite=True, drop_remainder=False):
    xfeed = cycle.utils.TFReader(FLAGS.Xtfr, normer=modellib.X_normer, denormer=modellib.X_denormer,
                                 batch_size=FLAGS.batchsize, infinite=infinite, drop_remainder=drop_remainder)
    yfeed = cycle.utils.TFReader(FLAGS.Ytfr, normer=modellib.Y_normer, denormer=modellib.Y_denormer,
                                 batch_size=FLAGS.batchsize, infinite=infinite, drop_remainder=drop_remainder)
    return xfeed, yfeed


if __name__ == '__main__':
    tf.app.run()
