import json
import math
import os
import random
import sys
import numpy as np
import tensorflow as tf
import matplotlib as mpl

from anime_dimension_reduction_keras import create_dataset, tf_data_generator

mpl.use('module://backend_interagg')
from keras.models import load_model
from cycle.models.anime._utils import normer, denormer
from keras.backend.tensorflow_backend import set_session

from cycle.utils import TFReader
from dimension_reduction_playground import extract_decoder

SIZE = (144, 256)


def norm_and_resize(data):
    data = normer(data)
    data = tf.image.resize_images(data, SIZE)
    return data


def denormalize(x):
    x_denormed = (((x + 1) / 2) * 255).astype(np.uint8)
    return x_denormed


def eval_model(name, validation_data):
    m = load_model(f'logs/{name}')
    reconst_data = m.predict(validation_data)
    l2_error = np.mean(np.abs(validation_data - reconst_data) ** 2)
    return l2_error


def main(_):
    seed = random.randrange(sys.maxsize)
    batch_size = 4
    dataset_name = '../../datasets/anime/no-game-no-life.tfrecord'
    data = create_dataset(dataset_name, batch_size, seed)
    iterator = data.make_one_shot_iterator()
    data_gen = tf_data_generator(iterator)

    # fuck it, I must create some validation data and keep it in memory, because fuck you
    validation_batches = 10
    # validation_data = [next(data_gen) for _ in range(validation_batches)]
    validation_data = np.array([next(data_gen) for _ in range(validation_batches)])

    models = ['2019-03-09--21-37', '2019-03-09--22-13',
              # '2019-03-09--22-50'
              ]
    params = {'2019-03-09--21-37': {'output': 'conv-k-9-d-3'},
              '2019-03-09--22-13': {'output': 'conv-k-9-d-16_conv-k-1-d-3'},
              # '2019-03-09--22-50': {'output': 'conv-k-9-d-3'}
              }
    eval_data = {}
    for model in models:
        eval_data[model] = eval_model(model, validation_data)


if __name__ == '__main__':
    tf.app.run()
