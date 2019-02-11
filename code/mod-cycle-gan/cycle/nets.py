#!/usr/bin/env python3

import tensorflow as tf
from . import _ops as ops
from tensorflow.contrib.gan.python.losses.python import losses_impl as gan_losses

_act_map = {'r': tf.nn.relu,
            't': tf.nn.tanh,
            'l': tf.nn.leaky_relu,
            's': tf.nn.sigmoid,
            'i': tf.identity}

_layer_map = {'c': ops.conv_block,
              'b': ops.res_block,
              'r': ops.reconv_block,
              'f': ops.fc}


# 'c-7-1-64-r;c-5-2-128-r;b-3-3-r;r-5-1-64-2-r;c-7-1-3-t;sc' - generator
# 'c-5-2-64-l;c-5-2-128-l;c-5-2-256-l;c-5-2-1-i;' - discriminator

class BaseNet(object):
    def __init__(self, name, network_desc, is_training, weight_lambda=0.0, norm='instance'):
        self.name = name
        self.is_training = is_training
        self.variables = None
        self.weight_lambda = weight_lambda
        self.norm = norm
        self.network_desc = network_desc.split(';')
        self.names = []
        self.layers_dicts = []  # added for debug
        self.inputs_cache = {}
        self.penultimate_inputs_cache = {}  # I need node before the last for discriminator bsc of stability (sigmoid)
        for i, n in enumerate(self.network_desc[:-1]):
            newname = n
            while True:
                for old_name in self.network_desc[:i]:
                    if newname == old_name:
                        newname += '-'
                        break
                else:
                    break
            self.names.append(newname)

    def build(self, data):
        # some computational graph optimization
        if data in self.inputs_cache:
            return self.inputs_cache[data]

        with tf.variable_scope(self.name, reuse=tf.AUTO_REUSE):
            result = self.transform(data)
        self.variables = tf.get_collection(tf.GraphKeys.TRAINABLE_VARIABLES, scope=self.name)
        self.inputs_cache[data] = result
        self.penultimate_inputs_cache[data] = result.op.inputs[0]  # penultimate layer (input to last layer)
        return result

    def build_penultimate(self, data):
        # some computational graph optimization
        if data in self.penultimate_inputs_cache:
            return self.penultimate_inputs_cache[data]

        self.build(data)
        return self.penultimate_inputs_cache[data]

    def transform(self, data):
        out = data
        layers_dict = {}
        for i, layer in enumerate(self.network_desc[:-1]):
            l_params = layer.split('-')
            out = _layer_map[l_params[0]](out, *tuple(map(ops.to_num, l_params[1:-1])),
                                          activation=_act_map[l_params[-1]],
                                          normtype=(self.norm if i < len(self.network_desc[:-1]) - 1 else None),
                                          is_training=self.is_training, name=self.names[i])
            layers_dict[layer] = out
        if self.network_desc[-1] != '':
            for op in self.network_desc[-1]:
                if op == 's':
                    out = out + data
                if op == 'c':
                    out = tf.clip_by_value(out, -1, 1)
                if op == 'a':
                    out = tf.nn.tanh(out)
                layers_dict[op] = out
        self.layers_dicts.append(layers_dict)
        return out

    def weight_loss(self):
        if self.variables is None:
            raise RuntimeError()
        if self.weight_lambda == 0:
            return 0
        vmean = [tf.reduce_mean(tf.square(v)) for v in self.variables]
        return tf.reduce_mean(vmean) * self.weight_lambda

    def num_params(self):
        if self.variables is None:
            raise RuntimeError()
        return tf.reduce_sum([tf.size(v) for v in self.variables])


class GAN(object):
    def __init__(self, gen: BaseNet, dis: BaseNet, in_shape, out_shape, tb_verbose, gen_lambda, dis_lambda,
                 selfreg_lambda, label_smoothing, selfreg_transform=None):
        assert isinstance(gen, BaseNet) and isinstance(dis, BaseNet)
        self.gen = gen
        self.dis = dis
        self.in_shape = in_shape
        self.out_shape = out_shape
        self.gen_lambda = gen_lambda
        self.dis_lambda = dis_lambda
        self.selfreg_lambda = selfreg_lambda
        self.selfreg_transform = selfreg_transform
        self.tb_verbose = tb_verbose
        self.real_label = 1 - label_smoothing
        self.label_smoothing = label_smoothing
        self.fake_label = 0

    def _gen_loss(self, data):
        return gan_losses.modified_generator_loss(discriminator_gen_outputs=data, add_summaries=self.tb_verbose)

    def selfreg_loss(self, orig, conv):
        if self.selfreg_lambda > 0:
            if self.selfreg_transform is not None:
                torig, tconv = self.selfreg_transform(orig, conv)
            else:
                torig, tconv = orig, conv
            return tf.reduce_mean(tf.abs(torig - tconv)) * self.selfreg_lambda
        return 0

    def gen_loss(self, orig_data):
        fake_dis_output = self.dis.build(self.gen.build(orig_data))
        with tf.name_scope('{}-gen-loss'.format(self.gen.name)):
            return self._gen_loss(fake_dis_output) * self.gen_lambda

    def _dis_loss(self, real, fake):
        return gan_losses.modified_discriminator_loss(
            discriminator_real_outputs=self.dis.build(real), discriminator_gen_outputs=self.dis.build(fake),
            label_smoothing=self.label_smoothing, add_summaries=self.tb_verbose)

    def dis_loss(self, real, fake):
        return self._dis_loss(real, fake) * self.dis_lambda

    def _construct_dis_full_loss(self, real, fake, name):
        pass

    def construct_dis_full_loss(self, real, fake, name):
        with tf.name_scope('{}-dis-loss'.format(name)):
            dis_loss = self.dis_loss(real, fake)
            dis_weight_loss = self.dis.weight_loss()
            dis_full_loss = dis_loss + dis_weight_loss

        if self.tb_verbose:
            tf.summary.scalar('{}_dis/dis_loss'.format(name), dis_loss)
            tf.summary.scalar('{}_dis/full_loss'.format(name), dis_full_loss)
            if self.dis.weight_lambda > 0:
                tf.summary.scalar('{}_dis/weight_loss'.format(name), dis_weight_loss)

        return dis_full_loss


class LSGAN(GAN):
    def _gen_loss(self, data):
        return gan_losses.least_squares_generator_loss(
            discriminator_gen_outputs=data, real_label=self.real_label, add_summaries=self.tb_verbose)

    def _dis_loss(self, real, fake):
        return gan_losses.least_squares_discriminator_loss(
            discriminator_gen_outputs=self.dis.build(fake), discriminator_real_outputs=self.dis.build(real),
            real_label=self.real_label, fake_label=self.fake_label, add_summaries=self.tb_verbose)


class WGAN(GAN):
    def __init__(self, gen: BaseNet, dis: BaseNet, in_shape, out_shape, tb_verbose, gen_lambda, dis_lambda, grad_lambda,
                 selfreg_lambda, label_smoothing, selfreg_transform=None, one_sided=False):
        super().__init__(gen, dis, in_shape, out_shape, tb_verbose, gen_lambda, dis_lambda, selfreg_lambda,
                         label_smoothing, selfreg_transform)
        self.grad_lambda = grad_lambda
        self.one_sided = one_sided

    def _gen_loss(self, data):
        return gan_losses.wasserstein_generator_loss(discriminator_gen_outputs=data, add_summaries=self.tb_verbose)

    def grad_loss(self, real, fake):
        def tfgan_dis(gen_data, not_used):
            return self.dis.build(gen_data)
        loss = gan_losses.wasserstein_gradient_penalty(
            real_data=real, generated_data=fake, generator_inputs=real, discriminator_fn=tfgan_dis,
            discriminator_scope=self.dis.name, one_sided=self.one_sided, add_summaries=self.tb_verbose)
        return self.grad_lambda * loss

    def dis_loss(self, real, fake):
        loss = gan_losses.wasserstein_discriminator_loss(
            discriminator_real_outputs=real, discriminator_gen_outputs=fake, real_weights=self.real_label,
            generated_weights=self.fake_label, add_summaries=self.tb_verbose)
        return self.dis_lambda * loss

    def _dis_loss(self, *args):
        raise NotImplementedError('WGAN is called differently, mate!')

    def construct_dis_full_loss(self, real, fake, name):
        with tf.name_scope('{}/dis_loss'.format(name)):
            dis_loss = self.dis_loss(real, fake)
            dis_weight_loss = self.dis.weight_loss()
            dis_grad_loss = self.grad_loss(real, fake)
            dis_full_loss = dis_loss + dis_weight_loss + dis_grad_loss

        if self.tb_verbose:
            tf.summary.scalar('{}_dis/dis_loss'.format(name), dis_loss)
            tf.summary.scalar('{}_dis/grad_loss'.format(name), dis_grad_loss)
            tf.summary.scalar('{}_dis/full_loss'.format(name), dis_full_loss)
            if self.dis.weight_lambda > 0:
                tf.summary.scalar('{}_dis/weight_loss'.format(name), dis_weight_loss)

        return dis_full_loss
