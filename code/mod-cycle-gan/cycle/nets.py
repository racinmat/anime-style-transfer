#!/usr/bin/env python3

import tensorflow as tf
from . import _ops as ops

_act_map = {'r' : tf.nn.relu,
            't' : tf.nn.tanh,
            'l' : tf.nn.leaky_relu,
            's' : tf.nn.sigmoid,
            'i' : tf.identity}

_layer_map = {'c' : ops.conv_block,
              'b' : ops.res_block,
              'r' : ops.reconv_block,
              'f' : ops.fc}

REAL_LABEL = 0.9

# 'c-7-1-64-r;c-5-2-128-r;b-3-3-r;r-5-1-64-2-r;c-7-1-3-t;sc' - generator
# 'c-5-2-64-l;c-5-2-128-l;c-5-2-256-l;c-5-2-1-s;' - discriminator

class BaseNet:
    def __init__(self, name, network_desc, is_training, weight_lambda=0.0, norm='instance'):
        self.name = name
        self.is_training = is_training
        self.variables = None
        self.weight_lambda = weight_lambda
        self.norm = norm
        self.network_desc = network_desc.split(';')
        self.names = []
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
        with tf.variable_scope(self.name, reuse=tf.AUTO_REUSE):
            result = self.transform(data)
        self.variables = tf.get_collection(tf.GraphKeys.TRAINABLE_VARIABLES, scope=self.name)
        return result


    def transform(self, data):
        out = data
        for i, layer in enumerate(self.network_desc[:-1]):
            l_params = layer.split('-')
            out = _layer_map[l_params[0]](out, *tuple(map(ops.to_num, l_params[1:-1])),
                                          activation=_act_map[l_params[-1]],
                                          normtype=(self.norm if i < len(self.network_desc[:-1]) - 1 else None),
                                          is_training=self.is_training, name=self.names[i])
        if self.network_desc[-1] != '':
            for op in self.network_desc[-1]:
                if op == 's':
                    out = out + data
                if op == 'c':
                    out = tf.clip_by_value(out, -1, 1)
                if op == 'a':
                    out = tf.nn.tanh(out)
        return out


    def weight_loss(self):
        if self.variables is None:
            raise RuntimeError()
        if self.weight_lambda == 0:
            return 0
        vmean = [tf.reduce_mean(tf.square(v)) for v in self.variables]
        return tf.reduce_mean(vmean) * self.weight_lambda


class GAN:
    def __init__(self, gen, dis, in_shape, out_shape,
                 gen_lambda, dis_lambda, selfreg_lambda, selfreg_transform=None):
        assert isinstance(gen, BaseNet) and isinstance(dis, BaseNet)
        self.gen = gen
        self.dis = dis
        self.in_shape = in_shape
        self.out_shape = out_shape
        self.gen_lambda = gen_lambda
        self.dis_lambda = dis_lambda
        self.selfreg_lambda = selfreg_lambda
        self.selfreg_transform = selfreg_transform


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
        return self._gen_loss(self.dis(self.gen(orig_data))) * self.gen_lambda


    def _dis_loss(self, real, fake):
        real_l = -tf.reduce_mean(ops.safe_log(self.dis(real)))
        fake_l = -tf.reduce_mean(ops.safe_log(1 - self.dis(fake)))
        return (real_l + fake_l)/2


    def dis_loss(self, real, fake):
        return self._dis_loss(real, fake) * self.dis_lambda


class LSGAN(GAN):
    def _gen_loss(self, data):
        return tf.reduce_mean(tf.squared_difference(data, REAL_LABEL))


    def _dis_loss(self, real, fake):
        real_l = tf.reduce_mean(tf.squared_difference(self.dis(real), REAL_LABEL))
        fake_l = tf.reduce_mean(tf.square(self.dis(fake)))
        return (real_l + fake_l)/2


class WGAN(GAN):
    def __init__(self, gen, dis, in_shape, out_shape,
                 gen_lambda, dis_lambda, grad_lambda, selfreg_lambda, selfreg_transform=None):
        super().__init__(gen, dis, in_shape, out_shape,
                         gen_lambda, dis_lambda, selfreg_lambda, selfreg_transform)
        self.grad_lambda = grad_lambda


    def _gen_loss(self, data):
        return -tf.reduce_mean(data)


    def grad_loss(self, real, fake):
        shape = [real.shape[0].value] + [1 for _ in range(len(real.shape)-1)]
        rand_eps = tf.random_uniform(shape=shape, minval=0., maxval=1.)
        orig_hat = real * rand_eps + fake * (1-rand_eps)
        grads = tf.gradients(self.dis(orig_hat), [orig_hat])
        return self.grad_lambda * tf.square(tf.norm(grads[0], ord=2) - 1.0)


    def dis_loss(self, real, fake):
        real_l = -tf.reduce_mean(self.dis(real))
        fake_l = tf.reduce_mean(self.dis(fake))
        return self.dis_lambda * (real_l + fake_l)/2


    def full_dis_loss(self, real, fake):
        return self.dis_loss(real, fake) + self.grad_loss(real, fake)


    def _dis_loss(self, *args):
        raise NotImplementedError('WGAN is called differently, mate!')
