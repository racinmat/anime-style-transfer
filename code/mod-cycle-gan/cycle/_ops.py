import tensorflow as tf

def safe_log(x, eps=1e-12):
    return tf.log(x + eps)

def weights(name, shape):
    return tf.get_variable(name, shape,
                          initializer=tf.contrib.layers.xavier_initializer())

def biases(name, shape, constant=0.0):
    return tf.get_variable(name, shape, initializer=tf.constant_initializer(constant))

def norm(data, is_training, normtype):
    if normtype.casefold() == 'instance'.casefold():
        return tf.contrib.layers.instance_norm(data)
    if normtype.casefold() == 'batch'.casefold():
        return tf.contrib.layers.batch_normalization(data, training=is_training, decay=0.9)
    return data

def conv_block(inputs, kernel_size, stride, out_channels, activation, normtype='instance', is_training=True, name=None, bias=False):
    with tf.variable_scope(name):
        kernel_shape = [kernel_size, kernel_size, inputs.get_shape()[3], out_channels]
        strides = [1, stride, stride, 1]
        c_out = tf.nn.conv2d(inputs, weights('weights', kernel_shape), strides, padding='SAME')
        if bias:
            c_out = c_out + biases('bias', c_out.get_shape())
        nc_out = norm(c_out, is_training, normtype)
        anc_out = activation(nc_out)
        return anc_out

def res_block(inputs, kernel_size, num_res, activation, normtype='instance', is_training=True, name=None, bias=False):
    with tf.variable_scope(name):
        out = inputs
        if name is None:
            name = ''
        for i in range(num_res):
            out = conv_block(out, kernel_size, 1, out.get_shape()[3], activation, normtype, is_training, name+str(i), bias)
        return inputs + out

def reconv_block(inputs, kernel_size, stride, out_channels, activation, out_size=None, normtype='instance', is_training=True, name=None, bias=False):
    with tf.variable_scope(name):
        inshape = inputs.get_shape().as_list()
        if out_size is None:
            out_size = [2*inshape[1], 2*inshape[2]]
        kernel_shape = [kernel_size, kernel_size, inshape[3], out_channels]
        strides = [1, stride, stride, 1]
        resized = tf.image.resize_images(inputs, out_size, tf.image.ResizeMethod.NEAREST_NEIGHBOR)
        c_out = tf.nn.conv2d(resized, weights('weights', kernel_shape), strides, padding='SAME')
        if bias:
            c_out = c_out + biases('bias', c_out.get_shape())
        nc_out = norm(c_out, is_training, normtype)
        anc_out = activation(nc_out)
        return anc_out
