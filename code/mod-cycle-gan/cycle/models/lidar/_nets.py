import tensorflow as tf
import cycle

# TODO: Paramtrizable generator/discriminator
class Generator(cycle.nets.BaseNet):
    def transform(self, data):
        first = cycle.ops.conv_block(data, 7, 1, 64, tf.nn.relu, self.norm, self.is_training, name='conv1')
        second = cycle.ops.conv_block(first, 5, 2, 128, tf.nn.relu, self.norm, self.is_training, name='conv2')
        res = cycle.ops.res_block(second, 3, 3, tf.nn.relu, self.norm, self.is_training, name='res')
        third = cycle.ops.reconv_block(res, 5, 1, 64, tf.nn.relu, normtype=self.norm, is_training=self.is_training, name='conv5')
        last = cycle.ops.conv_block(third, 7, 1, 3, tf.nn.tanh, self.norm, self.is_training, name='conv7')
        return tf.clip_by_value(last + data, -1, 1)

class Discriminator(cycle.nets.BaseNet):
    def transform(self, data):
        first = cycle.ops.conv_block(data, 5, 2, 64, tf.nn.leaky_relu, self.norm, self.is_training, name='conv1')
        second = cycle.ops.conv_block(first, 5, 2, 128, tf.nn.leaky_relu, self.norm, self.is_training, name='conv2')
        third = cycle.ops.conv_block(second, 5, 2, 256, tf.nn.leaky_relu, self.norm, self.is_training, name='conv3')
        last = cycle.ops.conv_block(third, 5, 2, 1, tf.nn.sigmoid, self.norm, self.is_training, name='conv5')
        return last
