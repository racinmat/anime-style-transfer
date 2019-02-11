import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt

plt.axis([0, 10, 0, 1])


def main():
    with tf.variable_scope('x'):
        x = tf.get_variable('x', shape=(), dtype=tf.float32, initializer=tf.constant_initializer(1))
    with tf.variable_scope('y'):
        y = tf.get_variable('y', shape=(), dtype=tf.float32, initializer=tf.constant_initializer(0))
    obj = x * y

    def history_averaging(v, l):
        return tf.reduce_mean(tf.square(v)) * l

    x_obj = -obj
    y_obj = obj

    opt_x = tf.train.GradientDescentOptimizer(learning_rate=2e-2)
    # opt_x = tf.train.AdamOptimizer(learning_rate=2e-2, beta1=0.5, beta2=0.8)
    # opt_x = tf.train.MomentumOptimizer(learning_rate=2e-2, momentum=0.1)
    x_mininize = opt_x.minimize(x_obj, var_list=[x])
    opt_y = tf.train.GradientDescentOptimizer(learning_rate=2e-2)
    # opt_y = tf.train.AdamOptimizer(learning_rate=2e-2, beta1=0.5, beta2=0.8)
    # opt_y = tf.train.MomentumOptimizer(learning_rate=2e-2, momentum=0.1)
    y_minimize = opt_y.minimize(y_obj, var_list=[y])

    sess = tf.InteractiveSession(config=tf.ConfigProto(device_count={'GPU': 0}))
    sess.run(tf.global_variables_initializer())
    fig1 = plt.subplot(1, 2, 1)
    fig1.set_title('variables values')
    fig2 = plt.subplot(1, 2, 2)
    fig2.set_title('objective function value')
    i = 0
    while True:
        # plt.cla()
        _, _, x_val, y_val, obj_val = sess.run([x_mininize, y_minimize, x, y, obj])
        fig1.scatter(x_val, y_val)
        fig2.scatter(i, obj_val)
        # plt.pause(0.0001)
        i += 1


if __name__ == '__main__':
    main()
