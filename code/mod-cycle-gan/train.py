#!/usr/bin/env python3

import importlib
import logging
import json
import tensorflow as tf
import cycle

def _valid_names(module):
    return [mod for mod in dir(module) if not mod.startswith('_')]

NEEDED_IMPORTS = ['XY_Discriminator', 'XY_Generator', 'YX_Discriminator', 'YX_Generator',
                  'X_denormer', 'X_normer', 'Y_denormer', 'Y_normer',
                  'X_DATA_SHAPE', 'Y_DATA_SHAPE']
FLAGS = tf.flags.FLAGS

# Flags for train
tf.flags.DEFINE_string('model', 'lidar', 'Model dir to use. Can choose from %s.\nNeeds to export {X,Y}_{normer,denormer} functions, {X,Y}_DATA_SHAPE and classes {XY,YX}_{Generator,Discriminator}.' % str(_valid_names(cycle.models)))
tf.flags.DEFINE_string('Xtfr', '/datagrid/personal/jasekota/dip-dataset/valeo/valeo-trn.interp.tfrecords', 'tfrecords file for X dataset')
tf.flags.DEFINE_string('Ytfr', '/datagrid/personal/jasekota/dip-dataset/gta/gta-trn.interp.tfrecords', 'tfrecords file for Y dataset')
tf.flags.DEFINE_string('gantype', 'LSGAN', 'Type of GAN to use within CycleGAN. Can choose from GAN/LSGAN/WGAN.\nWhile it is theoretically possible to use different GAN type for each part of training, it is generally not advised.')
tf.flags.DEFINE_float('XYgll', 2, 'Lambda for XY Generator loss')
tf.flags.DEFINE_float('YXgll', 2, 'Lambda for YX Generator loss')
tf.flags.DEFINE_float('XYdll', 2, 'Lambda for XY Discriminator loss')
tf.flags.DEFINE_float('YXdll', 2, 'Lambda for YX Discriminator loss')
tf.flags.DEFINE_float('XYgwl', 2, 'Lambda for XY Generator weight loss')
tf.flags.DEFINE_float('YXgwl', 2, 'Lambda for YX Generator weight loss')
tf.flags.DEFINE_float('XYdwl', 2, 'Lambda for XY Discriminator weight loss')
tf.flags.DEFINE_float('YXdwl', 2, 'Lambda for YX Discriminator weight loss')
tf.flags.DEFINE_float('cll', 2, 'Cycle loss lambda')
tf.flags.DEFINE_float('XYgrl', 2, 'Lambda for gradient loss for XY gan (applicable only if gantype is WGAN)')
tf.flags.DEFINE_float('YXgrl', 2, 'Lambda for gradient loss for YX gan (applicable only if gantype is WGAN)')
tf.flags.DEFINE_bool('visualise', True, 'Whether to visualize during training. in tensorboard. model than must have function visualise. Only valid if tbverbose.')
tf.flags.DEFINE_bool('tbverbose', True, 'Whether to export tensorboard data for visualization using Tensorboard.')
tf.flags.DEFINE_bool('verbose', True, 'Verbose mode.')
tf.flags.DEFINE_integer('dtsteps', 1, 'How many times will be trained discriminator during one step.')
tf.flags.DEFINE_integer('gtsteps', 1, 'How many times will be trained generator during one step.')
tf.flags.DEFINE_integer('batchsize', 1, 'Batch size')
tf.flags.DEFINE_string('norm', 'instance', 'One of two strings instance/batch.')
tf.flags.DEFINE_integer('steps', 200000, 'For how many steps will the model be trained.')
tf.flags.DEFINE_float('decayfrom', 0.5, 'Since when (as percentage of number of steps) will the learning rate start decaying.')
tf.flags.DEFINE_float('lr', 2e-4, 'Initial learning rate for adam optimizer.')
tf.flags.DEFINE_float('beta1', 0.5, 'Beta for optimizers.')
tf.flags.DEFINE_bool('history', True, 'Whether to keep history of generated images and sometimes swap them.')
tf.flags.DEFINE_integer('poolsize', 50, 'How large a history buffer will be. Valid only if history set to True.')


def main(_):
    logging.getLogger().setLevel(logging.INFO)
    flagdict = dict(((k, getattr(FLAGS, k)) for k in dir(FLAGS)))
    try:
        modellib = importlib.import_module('cycle.models.' + FLAGS.model)
        modelnames = _valid_names(modellib)
        found = [imp in modelnames for imp in NEEDED_IMPORTS]
        if not all(found):
            raise ImportError('Failed to find some necessary component in your model! The components not found are %s' % (str([imp for imp, f in zip(NEEDED_IMPORTS, found) if not f]),))
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
        xygen = modellib.XY_Generator(FLAGS.Xname + '-' + FLAGS.Yname + '-gen',
                                   True, FLAGS.XYgwl, FLAGS.norm)
        yxgen = modellib.YX_Generator(FLAGS.Yname + '-' + FLAGS.Xname + '-gen',
                                   True, FLAGS.YXgwl, FLAGS.norm)
        xydis = modellib.XY_Discriminator(FLAGS.Xname + '-' + FLAGS.Yname + '-dis',
                                       True, FLAGS.XYdwl, FLAGS.norm)
        yxdis = modellib.YX_Discriminator(FLAGS.Yname + '-' + FLAGS.Xname + '-dis',
                                       True, FLAGS.YXdwl, FLAGS.norm)

        if FLAGS.gantype.casefold() == 'GAN'.casefold():
            xy = cycle.nets.GAN(xygen, xydis, modellib.X_DATA_SHAPE, modellib.Y_DATA_SHAPE,
                                FLAGS.XYgll, FLAGS.XYdll)
            yx = cycle.nets.GAN(yxgen, yxdis, modellib.Y_DATA_SHAPE, modellib.X_DATA_SHAPE,
                                FLAGS.YXgll, FLAGS.YXdll)
        elif FLAGS.gantype.casefold() == 'LSGAN'.casefold():
            xy = cycle.nets.LSGAN(xygen, xydis, modellib.X_DATA_SHAPE, modellib.Y_DATA_SHAPE,
                                  FLAGS.XYgll, FLAGS.XYdll)
            yx = cycle.nets.LSGAN(yxgen, yxdis, modellib.Y_DATA_SHAPE, modellib.X_DATA_SHAPE,
                                  FLAGS.YXgll, FLAGS.YXdll)
        elif FLAGS.gantype.casefold() == 'WGAN'.casefold():
            xy = cycle.nets.WGAN(xygen, xydis, modellib.X_DATA_SHAPE, modellib.Y_DATA_SHAPE,
                                 FLAGS.XYgll, FLAGS.XYdll, FLAGS.XYgrl)
            yx = cycle.nets.WGAN(yxgen, yxdis, modellib.Y_DATA_SHAPE, modellib.X_DATA_SHAPE,
                                 FLAGS.YXgll, FLAGS.YXdll, FLAGS.YXgrl)
        else:
            raise ValueError('You did not specify any gantype that would be recognizible!')
        cygan = cycle.CycleGAN(xy, yx, xfeed, yfeed, FLAGS.Xname, FLAGS.Yname,
                               FLAGS.cll, FLAGS.tbverbose, True,
                               modellib.visualise if FLAGS.visualise else None,
                               FLAGS.lr, FLAGS.beta1, FLAGS.steps,
                               (FLAGS.decayfrom * FLAGS.steps), FLAGS.history)

        cygan.train(FLAGS.cpdir, FLAGS.gtsteps, FLAGS.dtsteps,
                    FLAGS.poolsize, FLAGS.rundir, FLAGS.verbose, json.dumps(flagdict, indent='\t'))



if __name__ == '__main__':
    tf.app.run()
