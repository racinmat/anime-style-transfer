import json
import math
import os
import random
import sys
import numpy as np
import tensorflow as tf
import matplotlib as mpl
import pandas as pd
from keras import Model
from plotly.offline import iplot, plot
import plotly.graph_objs as go
from anime_dimension_reduction_keras import create_dataset, tf_data_generator

mpl.use('module://backend_interagg')
from keras.models import load_model, model_from_json
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
    with open(f'logs/anime-{name}/model.json', encoding='utf-8') as f:
        m = model_from_json(f.read())  # type: Model
    m.load_weights(f'logs/anime-{name}/model.h5')
    reconst_data = m.predict(validation_data)
    l2_error = np.abs(validation_data - reconst_data) ** 2
    return np.mean(l2_error), np.median(l2_error)


def parallel_coordinates(df: pd.DataFrame):
    dimensions = []
    for col_name in df.columns:
        column = df[col_name]
        if '_encoded' in col_name:
            continue
        dimension = {
            'label': col_name,
        }

        if isinstance(column.dtype, pd.CategoricalDtype):
            dimension['range'] = [df[col_name + '_encoded'].min(), df[col_name + '_encoded'].max()]
            dimension['values'] = df[col_name + '_encoded'].values
            dimension['tickvals'] = np.unique(column.cat.codes)
            dimension['ticktext'] = column.cat.categories.values
        else:
            dimension['range'] = [column.min(), column.max()]
            dimension['values'] = column.values
        dimensions.append(dimension)
    data = [
        go.Parcoords(
            line=dict(color='blue'),
            # dimensions=list([
            #     dict(range=[1, 5],
            #          constraintrange=[1, 2],
            #          label='A', values=[1, 4]),
            #     dict(range=[1.5, 5],
            #          tickvals=[1.5, 3, 4.5],
            #          label='B', values=[3, 1.5]),
            #     dict(range=[1, 5],
            #          tickvals=[1, 2, 4, 5],
            #          label='C', values=[2, 4],
            #          ticktext=['text 1', 'text 2', 'text 3', 'text 4']),
            #     dict(range=[1, 5],
            #          label='D', values=[4, 2])
            # ]),
            dimensions=dimensions,
        )
    ]

    plot(data, filename='parallel_coords_hparams.html')


def main(_):
    seed = random.randrange(sys.maxsize)
    batch_size = 4
    dataset_name = '../../datasets/anime/no-game-no-life.tfrecord'
    data = create_dataset(dataset_name, batch_size, seed)
    iterator = data.make_one_shot_iterator()
    data_gen = tf_data_generator(iterator)

    # fuck it, I must create some validation data and keep it in memory, because fuck you
    validation_batches = 30
    # validation_data = [next(data_gen) for _ in range(validation_batches)]
    validation_data = np.array([next(data_gen)[0] for _ in range(validation_batches)])
    validation_data = validation_data.reshape((-1,) + validation_data.shape[2:])

    params = {
        '2019-03-09--21-37': {'dec': 'conv-k-9-d-3', 'activ': 'elu', 'init_lr': 5e-4, 'scale_k': 5},
        '2019-03-09--22-13': {'dec': 'conv-k-9-d-16_conv-k-1-d-3', 'activ': 'elu', 'init_lr': 5e-4, 'scale_k': 5},
        '2019-03-09--22-50': {'dec': 'conv-k-3-d-16_conv-k-1-d-3', 'activ': 'elu', 'init_lr': 5e-4, 'scale_k': 5},
        '2019-03-09--23-22': {'dec': 'deconv-k-9-d-3', 'activ': 'elu', 'init_lr': 5e-4, 'scale_k': 5},
        '2019-03-09--23-56': {'dec': 'deconv-k-3-d-16_deconv-k-1-d-3', 'activ': 'elu', 'init_lr': 5e-4, 'scale_k': 5},

        '2019-03-10--01-04': {'dec': 'conv-k-9-d-3', 'activ': 'relu', 'init_lr': 5e-4, 'scale_k': 5},
        '2019-03-10--01-39': {'dec': 'conv-k-9-d-16_conv-k-1-d-3', 'activ': 'relu', 'init_lr': 5e-4, 'scale_k': 5},
        '2019-03-10--02-16': {'dec': 'conv-k-3-d-16_conv-k-1-d-3', 'activ': 'relu', 'init_lr': 5e-4, 'scale_k': 5},
        '2019-03-10--02-47': {'dec': 'deconv-k-9-d-3', 'activ': 'relu', 'init_lr': 5e-4, 'scale_k': 5},
        '2019-03-10--03-19': {'dec': 'deconv-k-3-d-16_deconv-k-1-d-3', 'activ': 'relu', 'init_lr': 5e-4, 'scale_k': 5},


        '2019-03-10--16-59': {'dec': 'conv-k-9-d-3', 'activ': 'elu', 'init_lr': 1e-3, 'scale_k': 5},
        '2019-03-10--17-25': {'dec': 'conv-k-9-d-16_conv-k-1-d-3', 'activ': 'elu', 'init_lr': 1e-3, 'scale_k': 5},
        '2019-03-10--17-51': {'dec': 'conv-k-3-d-16_conv-k-1-d-3', 'activ': 'elu', 'init_lr': 1e-3, 'scale_k': 5},
        '2019-03-10--18-14': {'dec': 'deconv-k-9-d-3', 'activ': 'elu', 'init_lr': 1e-3, 'scale_k': 5},
        '2019-03-10--18-37': {'dec': 'deconv-k-3-d-16_deconv-k-1-d-3', 'activ': 'elu', 'init_lr': 1e-3, 'scale_k': 5},
        '2019-03-10--19-02': {'dec': 'deconv-k-9-d-16_deconv-k-1-d-3', 'activ': 'elu', 'init_lr': 1e-3, 'scale_k': 5},

        '2019-03-10--19-29': {'dec': 'conv-k-9-d-3', 'activ': 'relu', 'init_lr': 1e-3, 'scale_k': 5},
        '2019-03-10--19-54': {'dec': 'conv-k-9-d-16_conv-k-1-d-3', 'activ': 'relu', 'init_lr': 1e-3, 'scale_k': 5},
        '2019-03-10--20-21': {'dec': 'conv-k-3-d-16_conv-k-1-d-3', 'activ': 'relu', 'init_lr': 1e-3, 'scale_k': 5},
        '2019-03-10--20-43': {'dec': 'deconv-k-9-d-3', 'activ': 'relu', 'init_lr': 1e-3, 'scale_k': 5},
        '2019-03-10--21-07': {'dec': 'deconv-k-3-d-16_deconv-k-1-d-3', 'activ': 'relu', 'init_lr': 1e-3, 'scale_k': 5},
        '2019-03-10--21-31': {'dec': 'deconv-k-9-d-16_deconv-k-1-d-3', 'activ': 'relu', 'init_lr': 1e-3, 'scale_k': 5},


        '2019-03-10--22-58': {'dec': 'conv-k-9-d-3', 'activ': 'elu', 'init_lr': 5e-4, 'scale_k': 7},
        '2019-03-10--23-27': {'dec': 'conv-k-9-d-16_conv-k-1-d-3', 'activ': 'elu', 'init_lr': 5e-4, 'scale_k': 7},
        '2019-03-10--23-57': {'dec': 'conv-k-3-d-16_conv-k-1-d-3', 'activ': 'elu', 'init_lr': 5e-4, 'scale_k': 7},
        '2019-03-11--00-23': {'dec': 'deconv-k-9-d-3', 'activ': 'elu', 'init_lr': 5e-4, 'scale_k': 7},
        '2019-03-11--00-49': {'dec': 'deconv-k-3-d-16_deconv-k-1-d-3', 'activ': 'elu', 'init_lr': 5e-4, 'scale_k': 7},
        '2019-03-11--01-16': {'dec': 'deconv-k-9-d-16_deconv-k-1-d-3', 'activ': 'elu', 'init_lr': 5e-4, 'scale_k': 7},

        '2019-03-11--01-45': {'dec': 'conv-k-9-d-3', 'activ': 'relu', 'init_lr': 5e-4, 'scale_k': 7},
        '2019-03-11--02-13': {'dec': 'conv-k-9-d-16_conv-k-1-d-3', 'activ': 'relu', 'init_lr': 5e-4, 'scale_k': 7},
        '2019-03-11--02-42': {'dec': 'conv-k-3-d-16_conv-k-1-d-3', 'activ': 'relu', 'init_lr': 5e-4, 'scale_k': 7},
        '2019-03-11--03-06': {'dec': 'deconv-k-9-d-3', 'activ': 'relu', 'init_lr': 5e-4, 'scale_k': 7},
        '2019-03-11--03-32': {'dec': 'deconv-k-3-d-16_deconv-k-1-d-3', 'activ': 'relu', 'init_lr': 5e-4, 'scale_k': 7},
        '2019-03-11--03-58': {'dec': 'deconv-k-9-d-16_deconv-k-1-d-3', 'activ': 'relu', 'init_lr': 5e-4, 'scale_k': 7},
    }
    eval_data = []
    for model in params.keys():
        l2_error, l2_err_median = eval_model(model, validation_data)
        eval_data.append({**params[model], 'name': model, 'l2_error': l2_error, 'l2_error_inv': 1 / l2_error,
                          'l2_err_median': l2_err_median, 'l2_err_median_inv': 1/l2_err_median})

    df = pd.DataFrame(eval_data)
    df['activ'] = df['activ'].astype('category')
    df['dec'] = df['dec'].astype('category')
    df['name'] = df['name'].astype('category')
    df['activ_encoded'] = df['activ'].cat.codes
    df['dec_encoded'] = df['dec'].cat.codes
    df['name_encoded'] = df['name'].cat.codes

    parallel_coordinates(df)
    print('done')


if __name__ == '__main__':
    tf.app.run()
