import numpy as np
import tensorflow as tf
from keras.losses import mean_squared_error
from keras.models import Sequential, Model
from keras.layers import Dense, Input, Flatten, Reshape, Convolution2D, Convolution2DTranspose, Conv2D, Conv2DTranspose
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
        data = tf.image.resize_images(data, (432, 768))  # totally 331 776 pixels
        return data

    train.FLAGS.Xname = 'anime'
    train.FLAGS.Xtfr = '../../datasets/anime/no-game-no-life-ep-2.tfrecord'

    modellib.X_normer = norm_and_resize
    modellib.Y_normer = norm_and_resize
    batch_size = 8  # todo: check and try bigger
    train.FLAGS.batchsize = batch_size
    x_feed, y_feed = train.initialize_readers(modellib, infinite=True, drop_remainder=True)

    K.set_image_data_format('channels_last')  # set format

    z_size = 2
    regul_const = 10e-7
    m = Sequential()
    m.add(Conv2D(8, (3, 3), activation='elu', border_mode='valid', batch_input_shape=(batch_size, 432, 768, 3)))
    m.add(Conv2D(16, (3, 3), activation='elu', border_mode='valid'))
    m.add(Conv2D(32, (3, 3), activation='elu', border_mode='valid', name='bottleneck'))
    # m.add(Flatten(input_shape=(432, 768, 3)))
    # m.add(Dense(512, activation='elu'))
    # m.add(Dense(128, activation='elu'))
    # m.add(Dense(z_size, activation='linear', name='bottleneck', activity_regularizer=l1(regul_const)))
    # m.add(Dense(128, activation='elu'))
    # m.add(Dense(512, activation='elu'))
    # m.add(Reshape((432, 768, 3), name='decoder'))
    m.add(Conv2DTranspose(32, (3, 3), activation='elu', border_mode='valid'))
    m.add(Conv2DTranspose(16, (3, 3), activation='elu', border_mode='valid'))
    m.add(Conv2DTranspose(8, (3, 3), activation='elu', border_mode='valid'))
    m.add(Conv2D(3, (3, 3), activation='elu', border_mode='same'))
    # m.add(Dense(432*768*3, activation='linear'))
    m.compile(loss=mean_squared_error, optimizer=Adam())
    print(m.summary())
    tensorboard = TensorBoard(
        log_dir='logs/anime', histogram_freq=5, embeddings_freq=5, embeddings_layer_names=['bottleneck'],
        write_images=True, embeddings_data=x_feed.feed(), embeddings_metadata='embeddings.tsv')
    # config = tf.ConfigProto()
    # config.gpu_options.allow_growth = True
    # set_session(tf.Session(config=config))

    history = m.fit(x_feed.feed(), x_feed.feed(), steps_per_epoch=100, epochs=50, verbose=1,
                    validation_data=(x_feed.feed(), x_feed.feed()), validation_steps=4,
                    callbacks=[tensorboard]
                    )

    encoder = Model(m.input, m.get_layer('bottleneck').output)
    decoder = extract_decoder(m)
    z_pca = encoder.predict(x_feed.feed())  # bottleneck representation
    r_pca = x_feed.denormalize(m.predict(x_feed.feed()))

    show_factors(decoder, m.get_layer('bottleneck').units, 'anime', (768, 432))

    # visualize_data(x_train, y_train, r_pca_train, z_pca_train, 'train_anime' + suffix)
    # visualize_data(x_test, y_test, r_pca_test, z_pca_test, 'test_anime' + suffix)

    plot_network_history(history, 'anime')


if __name__ == '__main__':
    tf.app.run()
