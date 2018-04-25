import tensorflow as tf
import cycle

# TODO: Paramtrizable generator/discriminator
class Generator(cycle.nets.BaseNet):
    def transform(self, data):
        first = cycle.ops.conv_block(data, 7, 1, 32, tf.nn.relu, self.norm, self.is_training, name='conv1')
        second = cycle.ops.conv_block(first, 5, 2, 64, tf.nn.relu, self.norm, self.is_training, name='conv2')
        third = cycle.ops.conv_block(second, 3, 2, 128, tf.nn.relu, self.norm, self.is_training, name='conv3')
        res = cycle.ops.res_block(third, 3, 3, tf.nn.relu, self.norm, self.is_training, name='res')
        fifth = cycle.ops.reconv_block(res, 5, 2, 64, tf.nn.relu, normtype=self.norm, is_training=self.is_training, name='conv5')
        sixth = cycle.ops.reconv_block(fifth, 7, 2, 32, tf.nn.relu, normtype=self.norm, is_training=self.is_training, name='conv6')
        last = cycle.ops.conv_block(sixth, 7, 1, 3, tf.nn.tanh, self.norm, self.is_training, name='conv7')
        return last

class Discriminator(cycle.nets.BaseNet):
    def transform(self, data):
        first = cycle.ops.conv_block(data, 5, 2, 32, tf.nn.leaky_relu, self.norm, self.is_training, name='conv1')
        second = cycle.ops.conv_block(first, 5, 2, 64, tf.nn.leaky_relu, self.norm, self.is_training, name='conv2')
        third = cycle.ops.conv_block(second, 5, 2, 128, tf.nn.leaky_relu, self.norm, self.is_training, name='conv3')
        fourth = cycle.ops.conv_block(third, 5, 2, 256, tf.nn.leaky_relu, self.norm, self.is_training, name='conv4')
        last = cycle.ops.conv_block(fourth, 5, 2, 1, tf.nn.sigmoid, self.norm, self.is_training, name='conv5')
        return last
