#!/usr/bin/env python3

import tensorflow as tf
from . import _ops as ops
import abc

REAL_LABEL = 1

class BaseNet(abc.ABC):
    def __init__(self, name, is_training, weight_lambda, norm='instance'):
        self.name = name
        self.is_training = is_training
        self.variables = None
        self.weight_lambda = weight_lambda
        self.norm = norm


    def __call__(self, data):
        with tf.variable_scope(self.name, reuse=tf.AUTO_REUSE):
            result = self.transform(data)
        self.variables = tf.get_collection(tf.GraphKeys.TRAINABLE_VARIABLES, scope=self.name)
        return result


    @abc.abstractmethod
    def transform(self, data):
        raise NotImplementedError()


    def weight_loss(self):
        if self.variables is None:
            raise RuntimeError()
        if self.weight_lambda == 0:
            return 0
        vmean = [tf.reduce_mean(tf.square(v)) for v in self.variables]
        return tf.reduce_mean(vmean)


class GAN:
    def __init__(self, gen, dis, in_shape, out_shape, gen_lambda, dis_lambda):
        self.gen = gen
        self.dis = dis
        self.in_shape = in_shape
        self.out_shape = out_shape
        self.gen_lambda = gen_lambda
        self.dis_lambda = dis_lambda


    def _gen_loss(self, data):
        return -tf.reduce_mean(ops.safe_log(data))


    def gen_loss(self, orig_data):
        return self._gen_loss(self.dis(orig_data)) * self.gen_lambda


    def _dis_loss(self, orig_real, orig_fake):
        real_l = -tf.reduce_mean(ops.safe_log(self.dis(orig_real)))
        fake_l = -tf.reduce_mean(ops.safe_log(1-self.dis(orig_fake)))
        return (real_l + fake_l)/2


    def dis_loss(self, orig_real, orig_fake):
        return self._dis_loss(orig_real, orig_fake) * self.dis_lambda


class LSGAN(GAN):
    def _gen_loss(self, data):
        return tf.reduce_mean(tf.squared_difference(data, REAL_LABEL))


    def _dis_loss(self, orig_real, orig_fake):
        real_l = tf.reduce_mean(tf.squared_difference(self.dis(orig_real), REAL_LABEL))
        fake_l = tf.reduce_mean(tf.square(self.dis(orig_fake)))
        return (real_l + fake_l)/2


class WGAN(GAN):
    def __init__(self, gen, dis, in_shape, out_shape, gen_lambda, dis_lambda, grad_lambda):
        super().__init__(gen, dis, in_shape, out_shape, gen_lambda, dis_lambda)
        self.grad_lambda = grad_lambda


    def _gen_loss(self, data):
        return -tf.reduce_mean(data)


    def grad_loss(self, orig_real, orig_fake):
        shape = [orig_real.shape[0].value] + [1 for _ in range(len(orig_real.shape)-1)]
        rand_eps = tf.random_uniform(shape=shape, minval=0., maxval=1.)
        orig_hat = orig_real * rand_eps + orig_fake * (1-rand_eps)
        grads = tf.gradients(self.dis(orig_hat), [orig_hat])
        return self.grad_lambda * tf.square(tf.norm(grads[0], ord=2) - 1.0)


    def dis_loss(self, orig_real, orig_fake):
        real_l = -tf.reduce_mean(self.dis(orig_real))
        fake_l = tf.reduce_mean(self.dis(orig_fake))
        return self.dis_lambda * (real_l + fake_l)/2


    def full_dis_loss(self, orig_real, orig_fake):
        return self.dis_loss(orig_real, orig_fake) + self.grad_loss(orig_real, orig_fake)


    def _dis_loss(self, *args):
        raise(NotImplementedError('WGAN is called differently, mate!'))
