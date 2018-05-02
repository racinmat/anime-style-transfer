import tensorflow as tf

tf.flags.DEFINE_string('Xname', 'valeo', 'Name of the X dataset')
tf.flags.DEFINE_string('Yname', 'gta', 'Name of the Y dataset')
tf.flags.DEFINE_string('cpdir', '/datagrid/personal/jasekota/cycle-dip/cp', 'Toplevel checkpoint dir')
tf.flags.DEFINE_string('rundir', None, 'Directory within cpdir to either store/load info about run when in train mode (use None if you want to create new dir with timestamp) or where to find info if in test mode.\nIf None in test mode, it will use latest dir in cpdir.')
