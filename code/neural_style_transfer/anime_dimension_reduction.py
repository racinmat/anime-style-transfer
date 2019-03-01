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

from dimension_reduction_playground import extract_decoder, show_factors, plot_network_history


def main(_):
    modellib = train.import_model()
    def norm_and_resize(data):
        data = normer(data)
        data = tf.image.resize_images(data, (432, 768))   # totally 331 776 pixels
        return data

    modellib.X_normer = norm_and_resize
    modellib.Y_normer = norm_and_resize

    x_feed, y_feed = train.initialize_readers(modellib, infinite=False)

    config = tf.ConfigProto()
    config.gpu_options.allow_growth = True
    set_session(tf.Session(config=config))

    z_size = 2
    batch_size = 16     # todo: check and try bigger
    regul_const = 10e-7
    m = Sequential()
    m.add(Dense(512, activation='elu', input_shape=(331776,)))
    m.add(Dense(128, activation='elu'))
    m.add(Dense(z_size, activation='linear', name='bottleneck', activity_regularizer=l1(regul_const)))
    m.add(Dense(128, activation='elu'))
    m.add(Dense(512, activation='elu'))
    m.add(Dense(331776, activation='linear', name='decoder'))
    m.compile(loss='mean_squared_error', optimizer=Adam())
    tensorboard = TensorBoard(log_dir='logs/ae_cifar_100', histogram_freq=5)
    # print(m.summary())
    history = m.fit(x_feed.feed(), x_feed.feed(), steps_per_epoch=100, batch_size=batch_size, epochs=50, verbose=1,
                    validation_data=(x_feed.feed(), x_feed.feed()), callbacks=[tensorboard])

    encoder = Model(m.input, m.get_layer('bottleneck').output)
    decoder = extract_decoder(m)
    z_pca_train = encoder.predict(x_feed.feed())  # bottleneck representation
    z_pca_test = encoder.predict(y_feed.feed())  # bottleneck representation
    r_pca_train = x_feed.denormalize(m.predict(x_feed.feed()))
    r_pca_test = y_feed.denormalize(m.predict(x_feed.feed()))

    show_factors(decoder, m.get_layer('bottleneck').units, 'anime', (768, 432))

    # visualize_data(x_train, y_train, r_pca_train, z_pca_train, 'train_anime' + suffix)
    # visualize_data(x_test, y_test, r_pca_test, z_pca_test, 'test_anime' + suffix)

    plot_network_history(history, 'anime')


if __name__ == '__main__':
    tf.app.run()
