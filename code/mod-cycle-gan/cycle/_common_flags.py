import tensorflow as tf

tf.flags.DEFINE_string('Xname', None, 'Name of the X dataset. If training, can be set to None and then the name will be pulled from used model')
tf.flags.DEFINE_string('Yname', None, 'Name of the Y dataset. If training, can be set to None and then the name will be pulled from used model')
tf.flags.DEFINE_string('cpdir', '../../checkpoint', 'Toplevel checkpoint dir')
tf.flags.DEFINE_string('rundir', None, 'Directory within cpdir to either store/load info about run when in train mode (use None if you want to create new dir with timestamp) or where to find info if in test mode.\nIf None in test mode, it will use latest dir in cpdir.')
tf.flags.DEFINE_integer('gpu_idx', 0, 'GPU with which to run the network')
