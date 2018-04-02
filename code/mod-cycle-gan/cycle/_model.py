#!/usr/bin/env python3

import os
import os.path as osp
import logging
from datetime import datetime
import tensorflow as tf
from tensorflow import graph_util as gu
from . import _utils as utils

class CycleGAN:
    SAVE_NODES = ['input', 'output', 'd_input', 'd_output']
    def __init__(self, XtoY, YtoX, X_name='X', Y_name='Y',
                 X_feed=None, Y_feed=None,
                 cycle_lambda=10.0, tb_verbose=True, training=True, image_data=False,
                 learning_rate=2e-4, beta1=0.5, steps=2e5, decay_from=1e5,
                 graph=None, history=True):

        self.XtoY = XtoY
        self.YtoX = YtoX

        assert XtoY.in_shape == YtoX.out_shape and XtoY.out_shape == YtoX.in_shape

        self.X_name = X_name
        self.Y_name = Y_name
        self.X_feed = X_feed
        self.Y_feed = Y_feed

        self.cycle_lambda = cycle_lambda
        self.tb_verbose = tb_verbose
        self.training = training
        self.image_data = image_data

        self.learning_rate = learning_rate
        self.beta1 = beta1
        self.steps = steps
        self.decay_from = decay_from
        self.history = history

        if graph is None:
            self.graph = tf.get_default_graph()
        else:
            self.graph = graph

        with self.graph.as_default():
            if history:
                self.prev_fake_x = tf.placeholder(tf.float32, shape=XtoY.in_shape)
                self.prev_fake_y = tf.placeholder(tf.float32, shape=YtoX.in_shape)
            self.cur_x = tf.placeholder(tf.float32, shape=XtoY.in_shape)
            self.cur_y = tf.placeholder(tf.float32, shape=YtoX.in_shape)

        logging.info('Cycle GAN ready.\n'
            '%s_shape='+str(XtoY.in_shape)+'\t%s_shape='+str(YtoX.in_shape)+'\n'
            'cycle_lambda=%f\n'
            'learning_rate=%f\tbeta1=%f\tsteps=%d\tdecay_from=%d',
            X_name, Y_name, cycle_lambda, learning_rate, beta1, steps, decay_from)

    def get_model(self):
        with self.graph.as_default():
            cycle_loss = self._cycle_loss(self.cur_x, self.cur_y, self.cycle_lambda)

            fake_y = self.XtoY.gen(self.cur_x)
            fake_x = self.YtoX.gen(self.cur_y)

            x_gen_loss = self.YtoX.gen_loss(fake_x)
            y_gen_loss = self.XtoY.gen_loss(fake_y)

            x_weight_loss_gen = self.XtoY.gen.weight_loss()
            y_weight_loss_gen = self.YtoX.gen.weight_loss()

            x_weight_loss_dis = self.XtoY.dis.weight_loss()
            y_weight_loss_dis = self.YtoX.dis.weight_loss()

            if self.history:
                x_dis_loss = self.YtoX.dis_loss(self.cur_x, self.prev_fake_x)
            else:
                x_dis_loss = self.YtoX.dis_loss(self.cur_x, fake_x)
            x_dis_full_loss = x_dis_loss + x_weight_loss_dis

            if self.history:
                y_dis_loss = self.XtoY.dis_loss(self.cur_y, self.prev_fake_y)
            else:
                y_dis_loss = self.XtoY.dis_loss(self.cur_y, fake_y)
            y_dis_full_loss = y_dis_loss + y_weight_loss_dis

            x_gen_full_loss = x_gen_loss + x_weight_loss_gen + cycle_loss
            y_gen_full_loss = y_gen_loss + y_weight_loss_gen + cycle_loss

            if self.tb_verbose:
                X_dis_fake = self.YtoX.dis(fake_x)
                X_dis_real = self.YtoX.dis(self.cur_x)
                Y_dis_fake = self.XtoY.dis(fake_y)
                Y_dis_real = self.XtoY.dis(self.cur_y)

                tf.summary.histogram('D_{}/real'.format(self.X_name), X_dis_real)
                tf.summary.histogram('D_{}/fake'.format(self.X_name), X_dis_fake)
                tf.summary.histogram('D_{}/real'.format(self.Y_name), Y_dis_real)
                tf.summary.histogram('D_{}/fake'.format(self.Y_name), Y_dis_fake)

                tf.summary.scalar('{}_gen/weight_loss'.format(self.X_name), x_weight_loss_gen)
                tf.summary.scalar('{}_gen/gen_loss'.format(self.X_name), x_gen_loss)
                tf.summary.scalar('{}_gen/full_loss'.format(self.X_name), x_gen_full_loss)

                tf.summary.scalar('{}_gen/weight_loss'.format(self.Y_name), y_weight_loss_gen)
                tf.summary.scalar('{}_gen/gen_loss'.format(self.Y_name), y_gen_loss)
                tf.summary.scalar('{}_gen/full_loss'.format(self.Y_name), y_gen_full_loss)

                tf.summary.scalar('{}_dis/weight_loss'.format(self.X_name), x_weight_loss_dis)
                tf.summary.scalar('{}_dis/dis_loss'.format(self.X_name), x_dis_loss)
                tf.summary.scalar('{}_dis/full_loss'.format(self.X_name), x_dis_full_loss)

                tf.summary.scalar('{}_dis/weight_loss'.format(self.Y_name), y_weight_loss_dis)
                tf.summary.scalar('{}_dis/dis_loss'.format(self.Y_name), y_dis_loss)
                tf.summary.scalar('{}_dis/full_loss'.format(self.Y_name), y_dis_full_loss)

                tf.summary.scalar('cycle_loss', cycle_loss)

                if self.image_data:
                    tf.summary.image('{}->{}'.format(self.Y_name, self.X_name),
                                     utils.batch_convert2int(fake_x))
                    tf.summary.image('{}->{}->{}'.format(self.Y_name, self.X_name, self.Y_name),
                                     utils.batch_convert2int(self.XtoY.gen(fake_x)))
                    tf.summary.image('{}->{}'.format(self.X_name, self.Y_name),
                                     utils.batch_convert2int(fake_y))
                    tf.summary.image('{}->{}->{}'.format(self.X_name, self.Y_name, self.X_name),
                                     utils.batch_convert2int(self.YtoX.gen(fake_y)))
                else:
                    tf.summary.tensor_summary('{}->{}'.format(self.Y_name, self.X_name), fake_x)
                    tf.summary.tensor_summary('{}->{}->{}'.format(self.Y_name, self.X_name, self.Y_name),
                                              self.XtoY.gen(fake_x))
                    tf.summary.tensor_summary('{}->{}'.format(self.X_name, self.Y_name), fake_y)
                    tf.summary.tensor_summary('{}->{}->{}'.format(self.X_name, self.Y_name, self.X_name),
                                              self.YtoX(fake_y))

            with tf.control_dependencies(tf.get_collection(tf.GraphKeys.UPDATE_OPS)):
                if self.training:
                    with tf.variable_scope('training', reuse=tf.AUTO_REUSE):
                        global_step = tf.get_variable('global_step',
                                                      shape=(),
                                                      dtype=tf.int32,
                                                      initializer=tf.zeros_initializer)
                    x_gen_opt = self._optimizer(x_gen_full_loss,
                                                self.YtoX.gen.variables,
                                                global_step,
                                                'Adam/{}_gen'.format(self.X_name))
                    y_gen_opt = self._optimizer(y_gen_full_loss,
                                                self.XtoY.gen.variables,
                                                global_step,
                                                'Adam/{}_gen'.format(self.Y_name))
                    x_dis_opt = self._optimizer(x_dis_loss,
                                                self.YtoX.dis.variables,
                                                global_step,
                                                'Adam/{}_dis'.format(self.X_name))
                    y_dis_opt = self._optimizer(y_dis_loss,
                                                self.XtoY.dis.variables,
                                                global_step,
                                                'Adam/{}_dis'.format(self.Y_name))

                    with tf.control_dependencies([x_gen_opt, y_gen_opt]):
                        train_gen = tf.no_op('optimizers_gen')
                    with tf.control_dependencies([x_dis_opt, y_dis_opt]):
                        train_dis = tf.no_op('optimizers_dis')
                    global_step_op = tf.assign_add(global_step, 1)

                else:
                    train_gen = tf.no_op()
                    train_dis = tf.no_op()

            return {'train': {
                        'gen': train_gen,
                        'dis': train_dis,
                        'global_step': global_step_op
                    },
                    'losses': {
                        'cycle': cycle_loss,
                        '{}_gen_full'.format(self.X_name): x_gen_full_loss,
                        '{}_gen_full'.format(self.Y_name): y_gen_full_loss,
                        '{}_dis_full'.format(self.X_name): x_dis_full_loss,
                        '{}_dis_full'.format(self.Y_name): y_dis_full_loss,
                    },
                    'fakes': [fake_x, fake_y]}

    def train(self,
              checkpoints_dir,
              dis_train=1, gen_train=1,
              pool_size=50, load_model=None, log_verbose=True, export_final=True):
        if load_model is not None:
            full_checkpoints_dir = osp.join(checkpoints_dir, load_model)
        else:
            full_checkpoints_dir = osp.join(checkpoints_dir,
                                            datetime.now().strftime('%Y%m%d-%H%M'))
            i = 0
            checkpoints_dir_try = full_checkpoints_dir + '-' + str(i)
            while osp.exists(checkpoints_dir_try):
                i += 1
                checkpoints_dir_try = full_checkpoints_dir + '-' + str(i)
        os.makedirs(full_checkpoints_dir, exist_ok=True)

        with self.graph.as_default():
            model_ops = self.get_model()
            model_ops['summary'] = tf.summary.merge_all()
            train_writer = tf.summary.FileWriter(full_checkpoints_dir, self.graph)
            saver = tf.train.Saver()

        with tf.Session(graph=self.graph) as sess:
            if load_model is not None:
                checkpoint = tf.train.get_checkpoint_state(full_checkpoints_dir)
                meta_graph_path = checkpoint.model_checkpoint_path + '.meta'
                restore = tf.train.import_meta_graph(meta_graph_path)
                restore.restore(sess, tf.train.latest_checkpoint(full_checkpoints_dir))
                step = int(osp.basename(meta_graph_path).split('-')[1].split('.')[0])
            else:
                step = 0
                sess.run(tf.global_variables_initializer())

            coord = tf.train.Coordinator()
            threads = tf.train.start_queue_runners(sess=sess, coord=coord)

            if self.history:
                x_pool = utils.DataBuffer(pool_size, self.X_feed.batch_size)
                y_pool = utils.DataBuffer(pool_size, self.Y_feed.batch_size)

            try:
                if self.history:
                    cur_x, cur_y = sess.run([self.X_feed.feed(), self.Y_feed.feed()])
                while not coord.should_stop():
                    if self.history:
                        fx, fy = sess.run(model_ops['fakes'], feed_dict={
                            self.cur_x : cur_x,
                            self.cur_y : cur_y
                        })

                    cur_x, cur_y = sess.run([self.X_feed.feed(), self.Y_feed.feed()])
                    if self.history:
                        feeder_dict = {
                            self.cur_x : cur_x,
                            self.cur_y : cur_y,
                            self.prev_fake_x : x_pool.query(fx, step),
                            self.prev_fake_y : y_pool.query(fy, step)
                        }
                    else:
                        feeder_dict = {
                            self.cur_x : cur_x,
                            self.cur_y : cur_y
                        }
                    for _ in range(dis_train):
                        sess.run(model_ops['train']['dis'], feed_dict=feeder_dict)
                    for _ in range(gen_train):
                        sess.run(model_ops['train']['gen'], feed_dict=feeder_dict)
                    summary, _ = sess.run([model_ops['summary'], model_ops['train']['global_step']],
                                          feed_dict=feeder_dict)

                    train_writer.add_summary(summary, step)
                    train_writer.flush()

                    if log_verbose and step % 100 == 0:
                        logging.info('------ Step {:d} ------'.format(step))
                        losses = sess.run(model_ops['losses'], feed_dict=feeder_dict)
                        for k, v in losses:
                            logging.info('\t{}:\t{:8f}'.format(k, v))

                    if step % 10000 == 0:
                        save_path = saver.save(sess,
                                               full_checkpoints_dir + '/model.ckpt',
                                               global_step=step)
                        logging.info('Model saved in file: %s', save_path)

                    step += 1

                    if step >= self.steps:
                        logging.info('Stopping after %d iterations', self.steps)
                        coord.request_stop()
            except:
                if log_verbose:
                    logging.info('Interrupted')
                coord.request_stop()
            finally:
                save_path = saver.save(sess, checkpoints_dir + '/model.ckpt', global_step=step)
                if log_verbose:
                    logging.info('Model saved in file: %s', save_path)
                if export_final:
                    self.export(sess, full_checkpoints_dir)
                coord.request_stop()
                coord.join(threads)

    def export(self, sess, export_dir):
        self._export_one_part(sess, export_dir, True, '{}2{}.pb'.format(self.X_name, self.Y_name))
        self._export_one_part(sess, export_dir, False, '{}2{}.pb'.format(self.X_name, self.Y_name))

    def _export_one_part(self, sess, export_dir, XtoY, model_name):
        with self.graph.as_default():
            normer = self.X_feed.normalize if XtoY else self.Y_feed.normalize
            denormer = self.Y_feed.denormalize if XtoY else self.X_feed.denormalize
            data_in = normer(tf.placeholder(tf.float32,
                                    shape=self.XtoY.in_shape if XtoY else self.YtoX.out_shape,
                                    name='input'))
            out = self.XtoY.gen(data_in) if XtoY else self.YtoX.gen(data_in)
            d_in = self.XtoY.dis(data_in) if XtoY else self.YtoX.dis(data_in)
            d_out = self.YtoX.dis(out) if XtoY else self.XtoY.dis(out)
            denormer(out, name='output')
            tf.reduce_mean(d_in, 'd_input')
            tf.reduce_mean(d_out, 'd_output')

        output_graph_def = gu.extract_sub_graph(
                                gu.remove_training_nodes(
                                    gu.convert_variables_to_constants(
                                        sess, self.graph.as_graph_def(), CycleGAN.SAVE_NODES),
                                    CycleGAN.SAVE_NODES),
                                CycleGAN.SAVE_NODES)
        tf.train.write_graph(output_graph_def, export_dir, model_name, as_text=False)

    @staticmethod
    def export_from_checkpoint(XtoY, YtoX, X_normer, X_denormer, Y_normer, Y_denormer,
                               checkpoint_dir, export_dir,
                               X_name='X', Y_name='Y'):
        CycleGAN._export_cp_one_part(XtoY.gen, XtoY.dis, YtoX.dis,
                                     X_normer, Y_denormer,
                                     XtoY.in_shape,
                                     checkpoint_dir, export_dir, '{}2{}.pb'.format(X_name, Y_name))
        CycleGAN._export_cp_one_part(YtoX.gen, YtoX.dis, XtoY.dis,
                                     Y_normer, X_denormer,
                                     YtoX.in_shape,
                                     checkpoint_dir, export_dir, '{}2{}.pb'.format(Y_name, X_name))

    @staticmethod
    def _export_cp_one_part(gen,
                            out_dis, in_dis, normer, denormer,
                            in_shape,
                            cp_dir, export_dir, model_name):
        graph = tf.Graph()
        with graph.as_default():
            data_in = normer(tf.placeholder(tf.float32, shape=in_shape, name='input'))
            out = gen(data_in)
            d_in = in_dis(data_in)
            d_out = out_dis(out)
            denormer(out, name='output')
            tf.reduce_mean(d_in, 'd_input')
            tf.reduce_mean(d_out, 'd_output')
            restore = tf.train.Saver()

        with tf.Session(graph=graph) as sess:
            sess.run(tf.global_variables_initializer())
            cp = tf.train.latest_checkpoint(cp_dir)
            restore.restore(sess, cp)
            output_graph_def = gu.extract_sub_graph(
                                    gu.remove_training_nodes(
                                        gu.convert_variables_to_constants(
                                            sess, graph.as_graph_def(), CycleGAN.SAVE_NODES),
                                        CycleGAN.SAVE_NODES),
                                    CycleGAN.SAVE_NODES)
            tf.train.write_graph(output_graph_def, export_dir, model_name, as_text=False)

    def _optimizer(self, loss, variables, global_step, name):
        with tf.variable_scope(name):
            learning_rate = tf.where(tf.greater_equal(global_step, self.decay_from),
                                     tf.train.polynomial_decay(self.learning_rate,
                                                               global_step-self.decay_from,
                                                               self.steps-self.decay_from,
                                                               0, power=1.0),
                                     self.learning_rate)

            if self.tb_verbose:
                tf.summary.scalar('learning_rate/{}'.format(name), learning_rate)

            adam = tf.train.AdamOptimizer(learning_rate, beta1=self.beta1, name=name)
            learning_step = adam.minimize(loss, var_list=variables)
            return learning_step

    def _cycle_loss(self, x, y, cycle_lambda):
        y_to_y_diff = tf.abs(self.XtoY.gen(self.YtoX.gen(y)) - y)
        x_to_x_diff = tf.abs(self.YtoX.gen(self.XtoY.gen(x)) - x)
        return cycle_lambda * (tf.reduce_mean(x_to_x_diff) + tf.reduce_mean(y_to_y_diff))
