import numpy as np
import tensorflow as tf
from keras.models import Sequential, Model
from keras.layers import Dense, Input
from keras.optimizers import Adam
from keras import backend as K
from keras.callbacks import TensorBoard
from keras.regularizers import l1
import train
from cycle.models.anime._utils import normer
from keras.backend.tensorflow_backend import set_session


def main(_):
    modellib = train.import_model()
    # reshaping to static size so I can batch it
    def norm_and_resize(data):
        data = normer(data)
        data = tf.image.resize_images(data, 540, 960)   # totally 518 400 pixels
        return data

    modellib.X_normer = norm_and_resize
    modellib.Y_normer = norm_and_resize

    x_feed, y_feed = train.initialize_readers(modellib, infinite=False)

    config = tf.ConfigProto()
    config.gpu_options.allow_growth = True
    set_session(tf.Session(config=config))

    z_size = 2
    batch_size = 16 # todo: check
    regul_const = 10e-7
    m = Sequential()
    m.add(Dense(512, activation='elu', input_shape=(32**2,)))
    m.add(Dense(128, activation='elu'))
    m.add(Dense(z_size, activation='linear', name='bottleneck', activity_regularizer=l1(regul_const)))
    m.add(Dense(128, activation='elu'))
    m.add(Dense(512, activation='elu'))
    m.add(Dense(32**2, activation='linear', name='decoder'))
    m.compile(loss='mean_squared_error', optimizer=Adam())
    tensorboard = TensorBoard(log_dir='logs/ae_cifar_100', histogram_freq=5)
    print(m.summary())
    history = m.fit(x_feed, x_feed, batch_size=batch_size, epochs=50, verbose=1,
                    validation_data=(x_feed, x_feed), callbacks=[tensorboard])
    eval_show_network(m, mu_train, mu_test, x_train_normed, x_test_normed, y_train, y_test, history, 'ae_cifar_100', (32, 32))
    K.clear_session()


if __name__ == '__main__':
    tf.app.run()
