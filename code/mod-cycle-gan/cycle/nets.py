#!/usr/bin/env python3

import tensorflow as tf
from . import _ops as ops

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
# 'c-5-2-64-l;c-5-2-128-l;c-5-2-256-l;c-5-2-1-s;' - discriminator

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

    def __call__(self, data):
        # some computational graph optimization
        if data in self.inputs_cache:
            return self.inputs_cache[data]

        with tf.variable_scope(self.name, reuse=tf.AUTO_REUSE):
            result = self.transform(data)
        self.variables = tf.get_collection(tf.GraphKeys.TRAINABLE_VARIABLES, scope=self.name)
        self.inputs_cache[data] = result
        return result

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
                 selfreg_lambda, label_smoothing, one_sided_ls, selfreg_transform=None):
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
        self.fake_label = 0 if one_sided_ls else label_smoothing

    def _gen_loss(self, data):
        return -tf.reduce_mean(ops.safe_log(data))

    def selfreg_loss(self, orig, conv):
        if self.selfreg_lambda > 0:
            if self.selfreg_transform is not None:
                torig, tconv = self.selfreg_transform(orig, conv)
            else:
                torig, tconv = orig, conv
            return tf.reduce_mean(tf.abs(torig - tconv)) * self.selfreg_lambda
        return 0

    def gen_loss(self, orig_data):
        fake_dis_output = self.dis(self.gen(orig_data))
        with tf.name_scope('{}-gen-loss'.format(self.gen.name)):
            return self._gen_loss(fake_dis_output) * self.gen_lambda

    def _dis_loss(self, real, fake):
        real_l = -tf.reduce_mean(ops.safe_log(self.dis(real)))
        fake_l = -tf.reduce_mean(ops.safe_log(1 - self.dis(fake)))
        return (real_l + fake_l) / 2

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
        return tf.reduce_mean(tf.squared_difference(data, self.real_label))

    def _dis_loss(self, real, fake):
        real_l = tf.reduce_mean(tf.squared_difference(self.dis(real), self.real_label))
        fake_l = tf.reduce_mean(tf.square(self.dis(fake)))
        return (real_l + fake_l) / 2


class WGAN(GAN):
    def __init__(self, gen: BaseNet, dis: BaseNet, in_shape, out_shape, tb_verbose, gen_lambda, dis_lambda, grad_lambda,
                 selfreg_lambda, label_smoothing, one_sided_ls, selfreg_transform=None):
        super().__init__(gen, dis, in_shape, out_shape, tb_verbose, gen_lambda, dis_lambda, selfreg_lambda,
                         label_smoothing, one_sided_ls, selfreg_transform)
        self.grad_lambda = grad_lambda

    def _gen_loss(self, data):
        return -tf.reduce_mean(data)

    def grad_loss(self, real, fake):
        shape = [real.shape[0].value] + [1 for _ in range(len(real.shape) - 1)]
        rand_eps = tf.random_uniform(shape=shape, minval=0., maxval=1.)
        orig_hat = real * rand_eps + fake * (1 - rand_eps)
        grads = tf.gradients(ys=self.dis(orig_hat), xy=[orig_hat])
        return self.grad_lambda * tf.square(tf.norm(grads[0], ord=2) - 1.0)

    def dis_loss(self, real, fake):
        real_l = -tf.reduce_mean(self.dis(real))
        fake_l = tf.reduce_mean(self.dis(fake))
        return self.dis_lambda * (real_l + fake_l) / 2

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
