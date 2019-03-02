import numpy as np
import tensorflow as tf
from keras.losses import mean_squared_error
from keras.models import Sequential, Model
from keras.layers import Dense, Input, Flatten, Reshape, Convolution2D, Convolution2DTranspose, Conv2D, Conv2DTranspose, \
    ZeroPadding2D
from keras.optimizers import Adam
from keras import backend as K
from keras.callbacks import TensorBoard
from keras.regularizers import l1
import tensorflow.contrib.layers as layers
import tensorflow.contrib.slim as slim
from tensorflow.contrib.slim import arg_scope

from cycle.models.anime._utils import normer, denormer
from keras.backend.tensorflow_backend import set_session

from cycle.utils import TFReader
from dimension_reduction_playground import extract_decoder, show_factors, plot_network_history


def norm_and_resize(data):
    data = normer(data)
    data = tf.image.resize_images(data, (432, 768))  # totally 331 776 pixels
    return data


def create_dataset(tf_record, batch_size):
    data = tf.data.TFRecordDataset(tf_record)
    data = data.map(TFReader._parse_example_encoded, num_parallel_calls=8)
    data = data.map(norm_and_resize, num_parallel_calls=8)
    data = tf.data.Dataset.zip((data, data))
    data = data.apply(tf.data.experimental.shuffle_and_repeat(buffer_size=100))
    data = data.batch(batch_size, drop_remainder=True)
    data = data.prefetch(batch_size * 5)
    return data


def tf_data_generator(iterator):
    next_batch = iterator.get_next()
    while True:
        yield K.get_session().run(next_batch)


def main(_):
    batch_size = 4
    data = create_dataset('../../datasets/anime/no-game-no-life-ep-2.tfrecord', batch_size)
    iterator = data.make_one_shot_iterator()
    data_gen = tf_data_generator(iterator)

    K.set_image_data_format('channels_last')  # set format

    # fuck it, I must create some validation data and keep it in memory, because fuck you
    validation_batches = 4
    validation_data = [next(data_gen) for _ in range(validation_batches)]
    validation_data = (np.array([i[0] for i in validation_data]).reshape(-1, 432, 768, 3),
                       np.array([i[1] for i in validation_data]).reshape(-1, 432, 768, 3))

    z_size = 20
    regul_const = 10e-7
    input_tensor = Input(shape=(432, 768, 3))
    out = Conv2D(16, kernel_size=(3, 3), strides=(3, 3), activation='elu', border_mode='valid')(input_tensor)
    out = Conv2D(32, kernel_size=(3, 3), strides=(3, 3), activation='elu', border_mode='valid')(out)
    out = Conv2D(64, kernel_size=(3, 3), strides=(3, 3), activation='elu', border_mode='valid')(out)
    out = Conv2D(128, kernel_size=(3, 3), strides=(3, 3), activation='elu', border_mode='valid')(out)
    out = Flatten()(out)
    out = Dense(z_size, activation='linear', name='bottleneck', activity_regularizer=l1(regul_const))(out)
    out = Dense(8 * 14 * 64, activation='elu')(out)
    out = Reshape((8, 14, 64))(out)
    out = Conv2DTranspose(128, kernel_size=(2, 2), strides=(2, 2), activation='elu', padding='valid')(out)
    out = Conv2DTranspose(64, kernel_size=(3, 3), strides=(3, 3), activation='elu', padding='valid')(out)
    out = Conv2DTranspose(32, kernel_size=(3, 3), strides=(3, 3), activation='elu', padding='valid')(out)
    out = ZeroPadding2D(padding=(0, 2))(out)
    out = Conv2DTranspose(16, kernel_size=(3, 3), strides=(3, 3), activation='elu', padding='valid')(out)
    out = Conv2DTranspose(3, kernel_size=(1, 1), activation='elu', padding='same')(out)
    # m.add(Dense(432*768*3, activation='linear'))
    m = Model(inputs=input_tensor, outputs=out)
    m.compile(loss=mean_squared_error, optimizer=Adam())
    print(m.summary())
    tensorboard = TensorBoard(
        log_dir='logs/anime', histogram_freq=5, write_images=True,
        embeddings_freq=5, embeddings_layer_names=['bottleneck'],
        embeddings_data=validation_data[0]
    )
    config = tf.ConfigProto()
    config.gpu_options.allow_growth = True
    set_session(tf.Session(config=config))

    history = m.fit_generator(data_gen, steps_per_epoch=200, epochs=100, verbose=1,
                              validation_data=validation_data, validation_steps=validation_batches * batch_size,
                              callbacks=[tensorboard])

    model_json = m.to_json()
    with open("model.json", "w") as json_file:
        json_file.write(model_json)
    # serialize weights to HDF5
    m.save_weights("model.h5")

    encoder = Model(m.input, m.get_layer('bottleneck').output)
    decoder = extract_decoder(m)
    z_pca = encoder.predict(validation_data)  # bottleneck representation
    r_pca = denormer(m.predict(validation_data))

    show_factors(decoder, m.get_layer('bottleneck').units, 'anime', (768, 432))

    visualize_data(x_train, y_train, r_pca_train, z_pca_train, 'train_anime' + suffix)
    visualize_data(x_test, y_test, r_pca_test, z_pca_test, 'test_anime' + suffix)

    plot_network_history(history, 'anime')


if __name__ == '__main__':
    tf.app.run()
