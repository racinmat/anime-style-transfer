import functools
import json
import math
import os
import random
import sys
from contextlib import redirect_stdout
from datetime import datetime

import numpy as np
import tensorflow as tf
import matplotlib as mpl
from sklearn.decomposition import PCA
from tensorflow.python.util.serialization import get_json_type


mpl.use('module://backend_interagg')
from keras.losses import mean_squared_error
from keras.models import Sequential, Model
from keras.layers import Dense, Input, Flatten, Reshape, Conv2D, Conv2DTranspose, ZeroPadding2D, MaxPooling2D, \
    Cropping2D, LeakyReLU
from keras.optimizers import Adam
from anime_dimension_reduction_keras import SIZE, build_and_train_network


def prepare_network_1(lr, decay, activation='elu'):
    input_tensor = Input(shape=(SIZE[0], SIZE[1], 3))
    out = Conv2D(32, kernel_size=9, strides=1, activation=activation, padding='same', name='encoder_1')(input_tensor)
    out = Conv2D(64, kernel_size=7, strides=2, activation=activation, padding='same', name='encoder_2')(out)
    out = Conv2D(128, kernel_size=7, strides=2, activation=activation, padding='same', name='encoder_3')(out)
    out = Conv2DTranspose(64, kernel_size=7, strides=2, activation=activation, padding='same', name='decoder_3')(out)
    out = Conv2DTranspose(32, kernel_size=7, strides=2, activation=activation, padding='same', name='decoder_2')(out)
    out = Conv2D(3, kernel_size=9, activation='tanh', padding='same', name='decoder_1')(out)
    m = Model(inputs=input_tensor, outputs=out)
    m.compile(loss=mean_squared_error, optimizer=Adam(lr=lr, beta_1=0.9, beta_2=0.999,
                                                      epsilon=None, decay=decay, amsgrad=False))
    return m


def prepare_network_2(lr, decay, activation='elu'):
    input_tensor = Input(shape=(SIZE[0], SIZE[1], 3))
    out = Conv2D(32, kernel_size=9, strides=1, activation=activation, padding='same', name='encoder_1')(input_tensor)
    out = Conv2D(64, kernel_size=7, strides=2, activation=activation, padding='same', name='encoder_2')(out)
    out = Conv2D(128, kernel_size=7, strides=2, activation=activation, padding='same', name='encoder_3')(out)
    out = Conv2DTranspose(64, kernel_size=7, strides=2, activation=activation, padding='same', name='decoder_4')(out)
    out = Conv2DTranspose(32, kernel_size=7, strides=2, activation=activation, padding='same', name='decoder_3')(out)
    out = Conv2D(16, kernel_size=9, activation=activation, padding='same', name='decoder_2')(out)
    out = Conv2D(3, kernel_size=1, activation=activation, padding='same', name='decoder_1')(out)
    m = Model(inputs=input_tensor, outputs=out)
    m.compile(loss=mean_squared_error, optimizer=Adam(lr=lr, beta_1=0.9, beta_2=0.999,
                                                      epsilon=None, decay=decay, amsgrad=False))
    return m


def prepare_network_3(lr, decay, activation='elu'):
    input_tensor = Input(shape=(SIZE[0], SIZE[1], 3))
    out = Conv2D(32, kernel_size=3, strides=1, activation=activation, padding='same', name='encoder_1')(input_tensor)
    out = Conv2D(64, kernel_size=7, strides=2, activation=activation, padding='same', name='encoder_2')(out)
    out = Conv2D(128, kernel_size=7, strides=2, activation=activation, padding='same', name='encoder_3')(out)
    out = Conv2DTranspose(64, kernel_size=7, strides=2, activation=activation, padding='same', name='decoder_4')(out)
    out = Conv2DTranspose(32, kernel_size=7, strides=2, activation=activation, padding='same', name='decoder_3')(out)
    out = Conv2D(16, kernel_size=3, activation=activation, padding='same', name='decoder_2')(out)
    out = Conv2D(3, kernel_size=1, activation='tanh', padding='same', name='decoder_1')(out)
    m = Model(inputs=input_tensor, outputs=out)
    m.compile(loss=mean_squared_error, optimizer=Adam(lr=lr, beta_1=0.9, beta_2=0.999,
                                                      epsilon=None, decay=decay, amsgrad=False))
    return m


def prepare_network_4(lr, decay, activation='elu'):
    input_tensor = Input(shape=(SIZE[0], SIZE[1], 3))
    out = Conv2D(32, kernel_size=9, strides=1, activation=activation, padding='same', name='encoder_1')(input_tensor)
    out = Conv2D(64, kernel_size=7, strides=2, activation=activation, padding='same', name='encoder_2')(out)
    out = Conv2D(128, kernel_size=7, strides=2, activation=activation, padding='same', name='encoder_3')(out)
    out = Conv2DTranspose(64, kernel_size=7, strides=2, activation=activation, padding='same', name='decoder_3')(out)
    out = Conv2DTranspose(32, kernel_size=7, strides=2, activation=activation, padding='same', name='decoder_2')(out)
    out = Conv2DTranspose(3, kernel_size=9, activation='tanh', padding='same', name='decoder_1')(out)
    m = Model(inputs=input_tensor, outputs=out)
    m.compile(loss=mean_squared_error, optimizer=Adam(lr=lr, beta_1=0.9, beta_2=0.999,
                                                      epsilon=None, decay=decay, amsgrad=False))
    return m


def prepare_network_5(lr, decay, activation='elu'):
    input_tensor = Input(shape=(SIZE[0], SIZE[1], 3))
    out = Conv2D(32, kernel_size=3, strides=1, activation=activation, padding='same', name='encoder_1')(input_tensor)
    out = Conv2D(64, kernel_size=7, strides=2, activation=activation, padding='same', name='encoder_2')(out)
    out = Conv2D(128, kernel_size=7, strides=2, activation=activation, padding='same', name='encoder_3')(out)
    out = Conv2DTranspose(64, kernel_size=7, strides=2, activation=activation, padding='same', name='decoder_4')(out)
    out = Conv2DTranspose(32, kernel_size=7, strides=2, activation=activation, padding='same', name='decoder_3')(out)
    out = Conv2DTranspose(16, kernel_size=3, activation=activation, padding='same', name='decoder_2')(out)
    out = Conv2DTranspose(3, kernel_size=1, activation='tanh', padding='same', name='decoder_1')(out)
    m = Model(inputs=input_tensor, outputs=out)
    m.compile(loss=mean_squared_error, optimizer=Adam(lr=lr, beta_1=0.9, beta_2=0.999,
                                                      epsilon=None, decay=decay, amsgrad=False))
    return m


def prepare_network_6(lr, decay, activation='elu'):
    input_tensor = Input(shape=(SIZE[0], SIZE[1], 3))
    out = Conv2D(32, kernel_size=3, strides=1, activation=activation, padding='same', name='encoder_1')(input_tensor)
    out = Conv2D(64, kernel_size=7, strides=2, activation=activation, padding='same', name='encoder_2')(out)
    out = Conv2D(128, kernel_size=7, strides=2, activation=activation, padding='same', name='encoder_3')(out)
    out = Conv2DTranspose(64, kernel_size=7, strides=2, activation=activation, padding='same', name='decoder_4')(out)
    out = Conv2DTranspose(32, kernel_size=7, strides=2, activation=activation, padding='same', name='decoder_3')(out)
    out = Conv2DTranspose(16, kernel_size=9, activation=activation, padding='same', name='decoder_2')(out)
    out = Conv2DTranspose(3, kernel_size=1, activation='tanh', padding='same', name='decoder_1')(out)
    m = Model(inputs=input_tensor, outputs=out)
    m.compile(loss=mean_squared_error, optimizer=Adam(lr=lr, beta_1=0.9, beta_2=0.999,
                                                      epsilon=None, decay=decay, amsgrad=False))
    return m


def main(_):
    # seed = random.randrange(sys.maxsize)
    seed = 1297405518608473393
    batch_size = 4
    dataset_name = '../../datasets/anime/no-game-no-life.tfrecord'

    build_and_train_network(dataset_name, seed, batch_size, functools.partial(prepare_network_1, activation='elu'))
    build_and_train_network(dataset_name, seed, batch_size, functools.partial(prepare_network_2, activation='elu'))
    build_and_train_network(dataset_name, seed, batch_size, functools.partial(prepare_network_3, activation='elu'))
    build_and_train_network(dataset_name, seed, batch_size, functools.partial(prepare_network_4, activation='elu'))
    build_and_train_network(dataset_name, seed, batch_size, functools.partial(prepare_network_5, activation='elu'))
    build_and_train_network(dataset_name, seed, batch_size, functools.partial(prepare_network_6, activation='elu'))

    build_and_train_network(dataset_name, seed, batch_size, functools.partial(prepare_network_1, activation='relu'))
    build_and_train_network(dataset_name, seed, batch_size, functools.partial(prepare_network_2, activation='relu'))
    build_and_train_network(dataset_name, seed, batch_size, functools.partial(prepare_network_3, activation='relu'))
    build_and_train_network(dataset_name, seed, batch_size, functools.partial(prepare_network_4, activation='relu'))
    build_and_train_network(dataset_name, seed, batch_size, functools.partial(prepare_network_5, activation='relu'))
    build_and_train_network(dataset_name, seed, batch_size, functools.partial(prepare_network_6, activation='relu'))


if __name__ == '__main__':
    tf.app.run()
