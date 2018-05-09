import tensorflow as tf

MAX_RAYDIST = 131.0

def normer(data, name='normer'):
    tmp_data = data[:, :, 0] / MAX_RAYDIST
    ndata = tf.concat((tmp_data[:, :, None], data[:, :, 1:]), axis=-1)
    ndata = (ndata * 2) - 1
    return tf.identity(ndata, name=name)

def denormer(data, name='denormer'):
    ndata = (data + 1)/2
    tmp_data = ndata[:, :, 0] * MAX_RAYDIST
    return tf.concat((tmp_data[:, :, None], ndata[:, :, 1:]), axis=-1, name=name)

def _conv_visual(batch):
    data = tf.squeeze(batch)
    img = tf.image.convert_image_dtype((data[:, :, 0] + 1.0)/2.0, tf.uint8)
    zeros = tf.zeros_like(img)
    return tf.where(data[:, :, 2] > 0, x=img, y=zeros)

def visualize(data_orig, data_conv, data_conv_conv):
    img = tf.concat((_conv_visual(data_orig),
                    _conv_visual(data_conv),
                    _conv_visual(data_conv_conv)), axis=0)
    return img[None, :, :, None]
