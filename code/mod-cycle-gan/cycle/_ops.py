import tensorflow as tf
from tensorflow.contrib import layers


def to_num(s):
    n = float(s)
    return int(n) if int(n) == n else n


def safe_log(x, eps=1e-12):
    return tf.log(x + eps)


def weights(name, shape):
    return tf.get_variable(name, shape, initializer=layers.xavier_initializer())


def biases(name, shape, constant=0.0):
    return tf.get_variable(name, shape, initializer=tf.constant_initializer(constant))


def norm(data, is_training, normtype):
    if normtype is None:
        return data
    if normtype.casefold() == 'instance'.casefold():
        return layers.instance_norm(data)
    if normtype.casefold() == 'batch'.casefold():
        return layers.batch_norm(data, is_training=is_training, decay=0.9)
    if normtype.casefold() == 'layer'.casefold():
        return layers.layer_norm(data)
    return data


def conv_block(inputs, kernel_size, stride, out_channels, activation, normtype='instance', is_training=True, name=None,
               bias=False):
    with tf.variable_scope(name):
        kernel_shape = [kernel_size, kernel_size, inputs.get_shape()[3], out_channels]
        strides = [1, stride, stride, 1]
        c_out = tf.nn.conv2d(inputs, weights('weights', kernel_shape), strides, padding='SAME')
        if bias:
            c_out = c_out + biases('bias', c_out.shape[1:])
        nc_out = norm(c_out, is_training, normtype)
        anc_out = activation(nc_out)
        return anc_out


def fc(inputs, num_outputs, activation, normtype='instance', is_training=True, name=None, bias=False):
    with tf.variable_scope(name):
        fin = tf.reshape(inputs, [inputs.shape[0], -1])
        w = weights('weights', [fin.shape[1], num_outputs])
        c_out = tf.matmul(fin, w)
        if bias:
            c_out = c_out + biases('bias', c_out.shape[1:])
        nc_out = norm(c_out, is_training, normtype)
        anc_out = activation(nc_out)
        return anc_out


def res_block(inputs, kernel_size, num_res, activation, normtype='instance', is_training=True, name=None, bias=False):
    with tf.variable_scope(name):
        out = inputs
        for i in range(num_res):
            out = conv_block(out, kernel_size, 1, out.get_shape()[3], activation,
                             normtype, is_training, name + str(i), bias)
        return inputs + out


def reconv_block(inputs, kernel_size, stride, out_channels, out_coeff, activation, normtype='instance',
                 is_training=True, name=None, bias=False):
    with tf.variable_scope(name):
        inshape = inputs.get_shape().as_list()
        if inshape[1] is None and inshape[2] is None:
            inshape_dynamic = tf.shape(inputs)
            out_size = [tf.to_int32(out_coeff * inshape_dynamic[1]), tf.to_int32(out_coeff * inshape_dynamic[2])]
        else:
            out_size = [int(out_coeff * inshape[1]), int(out_coeff * inshape[2])]
        kernel_shape = [kernel_size, kernel_size, inshape[3], out_channels]
        strides = [1, stride, stride, 1]
        resized = tf.image.resize_images(inputs, out_size, tf.image.ResizeMethod.NEAREST_NEIGHBOR, align_corners=True)
        c_out = tf.nn.conv2d(resized, weights('weights', kernel_shape), strides, padding='SAME')
        if bias:
            c_out = c_out + biases('bias', c_out.shape[1:])
        nc_out = norm(c_out, is_training, normtype)
        anc_out = activation(nc_out)
        return anc_out
