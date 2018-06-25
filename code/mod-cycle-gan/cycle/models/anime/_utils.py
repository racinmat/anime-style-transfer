import tensorflow as tf


def normer(data, name='normer'):
    # input images are in range 0 to 255
    # output images are in range -1 to 1
    tmp_data = data / 255.
    ndata = (tmp_data * 2) - 1
    return tf.identity(ndata, name=name)


def denormer(data, name='denormer'):
    ndata = (data + 1) / 2
    tmp_data = ndata * 255
    return tf.identity(tmp_data, name=name)


def _conv_visual(batch):
    depth = tf.image.convert_image_dtype((batch[:, :, :, 0] + 1.0) / 2.0, tf.uint8)
    intensity = tf.image.convert_image_dtype((batch[:, :, :, 1] + 1.0) / 2.0, tf.uint8)
    zeros = tf.zeros_like(depth)
    return (tf.where(batch[:, :, :, 2] > 0, x=depth, y=zeros),
            tf.where(batch[:, :, :, 2] > 0, x=intensity, y=zeros))


def visualize(data_orig, data_conv, data_conv_conv, name_orig, name_conv):
    # data_conv like data_converted
    tf.summary.image('{}-orig'.format(name_orig), data_orig)
    tf.summary.image('{}2{}'.format(name_orig, name_conv), data_conv)
    tf.summary.image('{}2{}2{}'.format(name_orig, name_conv, name_orig), data_conv_conv)


def feature_map(orig, conv):
    odepth = orig[:, :, :, 0]
    ointensity = orig[:, :, :, 1]
    omask = orig[:, :, :, 2]
    cdepth = conv[:, :, :, 0]
    cintensity = conv[:, :, :, 1]
    cmask = conv[:, :, :, 2]
    zeros = tf.zeros_like(odepth)
    torig = tf.concat((tf.where((omask > 0) & (cmask > 0), odepth, zeros),
                       tf.where((omask > 0) & (cmask > 0), ointensity, zeros)), axis=-1)
    tconv = tf.concat((tf.where((omask > 0) & (cmask > 0), cdepth, zeros),
                       tf.where((omask > 0) & (cmask > 0), cintensity, zeros)), axis=-1)
    return torig, tconv
