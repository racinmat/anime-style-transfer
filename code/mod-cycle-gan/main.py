#!/usr/bin/env python3

import tensorflow as tf
import importlib
import cycle
import sys

def _valid_names(module):
    return [mod for mod in dir(module) if not mod.startswith('_')]

NEEDED_IMPORTS = ['X_normer', 'Y_normer', 'X_denormer', 'Y_denormer', 'DATA_SHAPE', 'BATCH_SIZE', 'Generator', 'Discriminator']
FLAGS = tf.flags.FLAGS

# Common flags
tf.flags.DEFINE_string('model', 'lidar', 'Model dir to use. Can choose from %s.\nNeeds to export {X,Y}_{normer,denormer} functions, DATA_SHAPE and BATCH_SIZE and classes Generator and Discriminator.' % str(_valid_names(cycle.models)))
tf.flags.DEFINE_string('op', 'train', 'Whether to train/test/export. Must be one of these three options')
tf.flags.DEFINE_string('Xname', 'valeo', 'Name of the X dataset')
tf.flags.DEFINE_string('Yname', 'gta', 'Name of the Y dataset')
tf.flags.DEFINE_string('cpdir', '/datagrid/personal/jasekota/cycle-dip/cp', 'Toplevel checkpoint dir')
tf.flags.DEFINE_string('rundir', None, 'Directory within cpdir to either store/load info about run when in train mode (use None if you want to create new dir with timestamp) or where to find info if in export or test mode.\nIf None in export/test mode, it will use latest dir in cpdir.')

#Flags for test op
tf.flags.DEFINE_string('Xin', '/datagrid/personal/jasekota/dip-dataset/valeo/valeo-tst.npy', 'Name of the X input npy file.')
tf.flags.DEFINE_string('Yin', '/datagrid/personal/jasekota/dip-dataset/gta/gta-tst.npy', 'Name of the Y input npy file.')
tf.flags.DEFINE_string('Xout', '/datagrid/personal/jasekota/dip-dataset/valeo/valeo-tst-out.npz', 'Name of the X output npz file.')
tf.flags.DEFINE_string('Yout', '/datagrid/personal/jasekota/dip-dataset/gta/gta-tst-out.npz', 'Name of the Y output npz file.')

# Flags for export op


# Flags for train
tf.flags.DEFINE_string('Xtfr', '/datagrid/personal/jasekota/dip-dataset/valeo/valeo-trn.tfrecords', 'tfrecords file for X dataset')
tf.flags.DEFINE_string('Ytfr', '/datagrid/personal/jasekota/dip-dataset/gta/gta-trn.tfrecords', 'tfrecords file for Y dataset')


def train():
    pass

def test():
    pass

def export():
    pass

def main(_):
    if FLAGS.op == 'train':
        train()
    elif FLAGS.op == 'export':
        export()
    elif FLAGS.op == 'test':
        test()
    else:
        raise(ValueError('Argument op must be one of the values train/export/test'))

if __name__ == '__main__':
    try:
        modellib = importlib.import_module('cycle.models.' + FLAGS.model)
        modelnames = _valid_names(modellib)
        found = [imp in modelnames for imp in NEEDED_IMPORTS]
        if not all(found):
            raise ImportError('Failed to find some necessary component in your model! The components not found are %s' % (str([imp for imp, f in zip(NEEDED_IMPORTS, found) if not f]),))
    except ImportError:
        print('Could not import your model ', FLAGS.model, '! Will not continue!', file=sys.stderr)
        raise
    else:
        tf.app.run()
