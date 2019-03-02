import numpy as np
import tensorflow as tf
import matplotlib as mpl

mpl.use('module://backend_interagg')
from keras.losses import mean_squared_error
from keras.models import Sequential, Model
from keras.layers import Dense, Input, Flatten, Reshape, Conv2D, Conv2DTranspose, ZeroPadding2D
from keras.optimizers import Adam
from keras import backend as K
from keras.callbacks import TensorBoard, Callback
from keras.regularizers import l1
from PIL import Image
import io
import tensorflow.contrib.layers as layers
import tensorflow.contrib.slim as slim
from tensorflow.contrib.slim import arg_scope
import matplotlib.pyplot as plt
from cycle.models.anime._utils import normer, denormer
from keras.backend.tensorflow_backend import set_session

from cycle.utils import TFReader
from dimension_reduction_playground import extract_decoder, plot_network_history


def norm_and_resize(data):
    data = normer(data)
    data = tf.image.resize_images(data, (432, 768))  # totally 331 776 pixels
    return data


def denormalize(x):
    x_denormed = (((x + 1) / 2) * 255).astype(np.uint8)
    return x_denormed


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


def make_image(arr):
    arr = denormalize(arr)
    height, width, channel = arr.shape
    image = Image.fromarray(arr)
    output = io.BytesIO()
    image.save(output, format='PNG')
    image_string = output.getvalue()
    output.close()
    return tf.Summary.Image(height=height,
                            width=width,
                            colorspace=channel,
                            encoded_image_string=image_string)


class TensorBoardImage(Callback):
    def __init__(self, model: Model, data):
        super().__init__()
        self.model = model
        self.data = data
        self.writer = None

    def on_epoch_end(self, epoch, logs={}):
        self.writer = tf.summary.FileWriter('./logs')
        reconst_images = self.model.predict(self.data)
        for image_orig, image_reconst in zip(self.data, reconst_images):
            summary_orig = tf.Summary(value=[tf.Summary.Value(tag='orig_data', image=make_image(image_orig))])
            summary_reconst = tf.Summary(
                value=[tf.Summary.Value(tag='reconstructed_data', image=make_image(image_reconst))])
            self.writer.add_summary(summary_orig, epoch)
            self.writer.add_summary(summary_reconst, epoch)
        self.writer.close()

        return


def show_data(data, name):
    data = denormalize(data)
    plt.figure(figsize=(20, 20))
    plt.axis('off')
    plt.title(name, fontsize='50')
    size = data.shape[0]
    tile_width = int(round(np.sqrt(size)))
    while size % tile_width != 0:
        tile_width -= 1

    nw = size // tile_width
    # aligning images to one big
    h, w = (432, 768)
    tiled = data.reshape(tile_width, nw, h, w, 3).swapaxes(1, 2).reshape(tile_width * h, nw * w, 3)

    plt.imshow(tiled)
    plt.savefig(f'figures/{name}.png')
    plt.show()


def visualize_data(x_orig, x_reconst, z, suffix):
    show_data(x_orig, 'x_orig_' + suffix)
    show_data(x_reconst, 'x_reconst_' + suffix)

    plt.title('z_' + suffix)
    # plt.axis('off')   # I want to see the scale
    plt.scatter(x=z[:, 0], y=z[:, 1], s=10)
    plt.savefig(f'figures/z_{suffix}.png')
    plt.show()

    # sns.jointplot(x=z_pca_train[:, 0], y=z_pca_train[:, 1])
    # plt.title('z_' + suffix)
    # plt.show()


def show_factors(decoder, z_size, suffix):
    for i in range(z_size):
        latent_vector = np.zeros((1, z_size))
        latent_vector[:, i] = 1
        plt.imshow(denormalize(decoder.predict(latent_vector)[0]))
        plt.axis('off')
        plt.savefig(f'figures/factor-{suffix}-{i}.png')
        plt.show()


def main(_):
    batch_size = 4
    data = create_dataset('../../datasets/anime/no-game-no-life-ep-2.tfrecord', batch_size)
    iterator = data.make_one_shot_iterator()
    data_gen = tf_data_generator(iterator)

    K.set_image_data_format('channels_last')  # set format

    # fuck it, I must create some validation data and keep it in memory, because fuck you
    validation_batches = 10
    validation_data = [next(data_gen) for _ in range(validation_batches)]
    validation_data = (np.array([i[0] for i in validation_data]).reshape(-1, 432, 768, 3),
                       np.array([i[1] for i in validation_data]).reshape(-1, 432, 768, 3))

    z_size = 20
    regul_const = 10e-7
    input_tensor = Input(shape=(432, 768, 3))
    out = Conv2D(16, kernel_size=(3, 3), strides=(3, 3), activation='elu', padding='valid')(input_tensor)
    out = Conv2D(32, kernel_size=(3, 3), strides=(3, 3), activation='elu', padding='valid')(out)
    out = Conv2D(64, kernel_size=(3, 3), strides=(3, 3), activation='elu', padding='valid')(out)
    out = Conv2D(128, kernel_size=(3, 3), strides=(3, 3), activation='elu', padding='valid')(out)
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
    m = Model(inputs=input_tensor, outputs=out)
    m.compile(loss=mean_squared_error, optimizer=Adam())
    print(m.summary())
    tensorboard = TensorBoard(
        log_dir='logs/anime', histogram_freq=5, write_images=True, embeddings_freq=5,
        embeddings_layer_names=['bottleneck'], embeddings_data=validation_data[0]
    )
    tbi_callback = TensorBoardImage(model=m, data=validation_data[0])
    config = tf.ConfigProto()
    config.gpu_options.allow_growth = True
    set_session(tf.Session(config=config))

    history = m.fit_generator(data_gen, steps_per_epoch=20, epochs=1, verbose=1, validation_data=validation_data,
                              validation_steps=validation_batches * batch_size, callbacks=[tensorboard, tbi_callback])

    model_json = m.to_json()
    with open("model.json", "w") as json_file:
        json_file.write(model_json)
    # serialize weights to HDF5
    m.save_weights("model.h5")

    encoder = Model(m.input, m.get_layer('bottleneck').output)
    decoder = extract_decoder(m)
    latent_space = encoder.predict(validation_data[0])  # bottleneck representation
    reconst_data = m.predict(validation_data[0])

    show_factors(decoder, m.get_layer('bottleneck').units, 'anime')
    plot_network_history(history, 'anime')
    visualize_data(validation_data[0], reconst_data, latent_space, 'train_anime')


if __name__ == '__main__':
    tf.app.run()
