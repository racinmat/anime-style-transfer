import numpy as np
import tensorflow as tf
import sys
import matplotlib.pyplot as plt
from keras.datasets import mnist
from keras.models import Sequential, Model
from keras.layers import Dense
from keras.optimizers import Adam


def normalize(x):
    x_normed = (x / 255).astype(np.float32) * 2 - 1
    means = x_normed.mean(axis=0)
    # means = 0
    return x_normed, means


def denormalize(x, means):
    return (((x + means - 1) / 2) * 255).astype(np.int32)


def show_data(data, name):
    plt.figure()
    plt.title(name)
    size = data.shape[0]
    tile_width = int(round(np.sqrt(size)))
    while size % tile_width != 0:
        tile_width -= 1

    nw = size // tile_width
    # aligning images to one big
    h, w = 28, 28
    tiled = data.reshape(tile_width, nw, h, w).swapaxes(1, 2).reshape(tile_width * h, nw * w)

    plt.imshow(tiled, cmap="gray")
    plt.show()


def main(_):
    (x_train, y_train), (x_test, y_test) = mnist.load_data()
    x_train = x_train.reshape(60000, 784)[:100, :]
    x_test = x_test.reshape(10000, 784)[:100, :]
    x_train_normed, mu_train = normalize(x_train)
    x_test_normed, mu_test = normalize(x_test)

    # numpy pure pca
    u, s, v = np.linalg.svd(x_train_normed, full_matrices=False)
    z_pca_train = np.dot(x_train_normed, v.transpose())
    z_pca_test = np.dot(x_test_normed, v.transpose())

    r_pca_train = denormalize(np.dot(z_pca_train[:, :2], v[:2, :]), mu_train)  # reconstruction
    r_pca_test = denormalize(np.dot(z_pca_test[:, :2], v[:2, :]), mu_test)  # reconstruction
    err_train = np.sum((x_train - r_pca_train) ** 2) / r_pca_train.size
    err_test = np.sum((x_test - r_pca_test) ** 2) / r_pca_test.size
    print('PCA train reconstruction error with 2 PCs: ' + str(round(err_train, 3)))
    print('PCA test reconstruction error with 2 PCs: ' + str(round(err_test, 3)))

    show_data(x_train, 'x_train_orig')
    show_data(x_test, 'x_test_orig')

    show_data(r_pca_train, 'x_train_reconst')
    show_data(r_pca_test, 'x_test_reconst')
    # tensorflow pca
    tf.enable_eager_execution()
    x_tf = x_train.copy()

    mu_tf = tf.reduce_mean(x_tf, axis=0)
    s_tf, u_tf, v_tf = tf.svd(x_tf - mu_tf, full_matrices=False)

    z_pca_tf = tf.tensordot(x_tf - mu_tf, tf.transpose(v_tf), axes=1)
    r_pca_tf = tf.tensordot(z_pca_tf[:, :2], v_tf[:2, :], axes=1) + mu_tf  # reconstruction
    err_tf = tf.reduce_sum((x_tf - r_pca_tf) ** 2) / r_pca_tf.numpy().size
    print('PCA reconstruction error with 2 PCs in tensorflow: ' + str(round(err_tf.numpy(), 3)))

    print('done')


if __name__ == '__main__':
    tf.app.run()
