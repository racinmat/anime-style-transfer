import numpy as np
import tensorflow as tf
import pandas as pd
import sys
import matplotlib as mpl

mpl.use('module://backend_interagg')
import matplotlib.pyplot as plt
import seaborn as sns
from keras.datasets import mnist
from keras.models import Sequential, Model
from keras.layers import Dense
from keras.optimizers import Adam
from sklearn.decomposition import PCA, IncrementalPCA


def normalize(x):
    x_normed = (x / 255) * 2 - 1
    means = x_normed.mean(axis=0)
    # means = 0
    return x_normed - means, means


def denormalize(x, means):
    x += means
    x_denormed = (((x + 1) / 2) * 255).astype(np.int32)
    return x_denormed


def visualize_data(x_orig, y_orig, x_reconst, z, suffix, ims_limit=1000):
    show_data(x_orig[:ims_limit, :], 'x_orig_' + suffix)
    show_data(x_reconst[:ims_limit, :], 'x_reconst_' + suffix)

    plt.title('z_' + suffix)
    plt.axis('off')
    sc = plt.scatter(x=z[:, 0], y=z[:, 1], s=10, c=y_orig, cmap=plt.cm.Set1)
    plt.colorbar(sc)
    plt.show()

    # sns.jointplot(x=z_pca_train[:, 0], y=z_pca_train[:, 1])
    # plt.title('z_' + suffix)
    # plt.show()


def show_data(data, name):
    plt.figure(figsize=(20, 20))
    plt.axis('off')
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
    # plt.draw()
    plt.show()


def main(_):
    z_size = 2
    (x_train, y_train), (x_test, y_test) = mnist.load_data()
    x_train = x_train.reshape(-1, 784)
    y_train = y_train
    x_test = x_test.reshape(-1, 784)
    x_train_normed, mu_train = normalize(x_train)
    x_test_normed, mu_test = normalize(x_test)

    # numpy pure pca
    #####################################################################
    # for PCA it is important to have 0 mean otherwise it does not work #
    #####################################################################
    u, s, v = np.linalg.svd(x_train_normed, full_matrices=False)
    z_pca_train = (x_train_normed @ v.T)[:, :z_size]
    z_pca_test = (x_test_normed @ v.T)[:, :z_size]

    r_pca_train = denormalize(z_pca_train @ v[:z_size, :], mu_train)  # reconstruction
    r_pca_test = denormalize(z_pca_test @ v[:z_size, :], mu_test)  # reconstruction
    err_train = np.sum((x_train - r_pca_train).astype(np.int64) ** 2) / r_pca_train.size
    err_test = np.sum((x_test - r_pca_test).astype(np.int64) ** 2) / r_pca_test.size
    print('PCA train reconstruction error with 2 PCs: ' + str(round(err_train, 3)))
    print('PCA test reconstruction error with 2 PCs: ' + str(round(err_test, 3)))

    for i in range(z_size):
        plt.imshow(v.reshape(-1, 28, 28)[i], cmap="gray")
        plt.show()

    visualize_data(x_train, y_train, r_pca_train, z_pca_train, 'train')
    visualize_data(x_test, y_test, r_pca_test, z_pca_test, 'test')

    # scikit-learn pca
    pca = PCA(n_components=z_size)
    z_pca_train = pca.fit_transform(x_train)
    z_pca_test = pca.transform(x_test)
    r_pca_train = pca.inverse_transform(z_pca_train)
    r_pca_test = pca.inverse_transform(z_pca_test)
    err_train = np.sum((x_train - r_pca_train).astype(np.int64) ** 2) / r_pca_train.size
    err_test = np.sum((x_test - r_pca_test).astype(np.int64) ** 2) / r_pca_test.size
    print('scikit-learn PCA train reconstruction error with 2 PCs: ' + str(round(err_train, 3)))
    print('scikit-learn PCA test reconstruction error with 2 PCs: ' + str(round(err_test, 3)))

    for i in range(z_size):
        plt.imshow(pca.components_.reshape(-1, 28, 28)[i], cmap="gray")
        plt.show()

    visualize_data(x_train, y_train, r_pca_train, z_pca_train, 'train')
    visualize_data(x_test, y_test, r_pca_test, z_pca_test, 'test')

    # scikit-learn incremental pca
    pca = IncrementalPCA(n_components=z_size, batch_size=100)
    z_pca_train = pca.fit_transform(x_train)
    z_pca_test = pca.transform(x_test)
    r_pca_train = pca.inverse_transform(z_pca_train)
    r_pca_test = pca.inverse_transform(z_pca_test)
    err_train = np.sum((x_train - r_pca_train).astype(np.int64) ** 2) / r_pca_train.size
    err_test = np.sum((x_test - r_pca_test).astype(np.int64) ** 2) / r_pca_test.size
    print('scikit-learn incremental PCA train reconstruction error with 2 PCs: ' + str(round(err_train, 3)))
    print('scikit-learn incremental PCA test reconstruction error with 2 PCs: ' + str(round(err_test, 3)))

    for i in range(z_size):
        plt.imshow(pca.components_.reshape(-1, 28, 28)[i], cmap="gray")
        plt.show()

    visualize_data(x_train, y_train, r_pca_train, z_pca_train, 'train')
    visualize_data(x_test, y_test, r_pca_test, z_pca_test, 'test')

    # keras pca using autoencoder
    tf.enable_eager_execution()
    x_tf = x_train_normed.astype(np.float32).copy()

    mu_tf = tf.reduce_mean(x_tf, axis=0)
    s_tf, u_tf, v_tf = tf.svd(x_tf - mu_tf, full_matrices=False)

    z_pca_tf = tf.tensordot(x_tf - mu_tf, tf.transpose(v_tf), axes=1)
    r_pca_tf = tf.tensordot(z_pca_tf[:, :2], v_tf[:2, :], axes=1) + mu_tf  # reconstruction
    err_tf = tf.reduce_sum((x_tf - r_pca_tf) ** 2) / r_pca_tf.numpy().size
    print('PCA reconstruction error with 2 PCs in tensorflow: ' + str(round(err_tf.numpy(), 3)))

    print('done')


if __name__ == '__main__':
    tf.app.run()
