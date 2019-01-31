#!/usr/bin/env python3

import os
import os.path as osp
import logging
import time
from datetime import datetime

import progressbar
import tensorflow as tf
from tensorflow import graph_util as gu
import numpy as np
from . import utils, nets


class CycleGAN:
    SAVE_NODES = ['input', 'output', 'd_input', 'd_output']
    OUTPUT_NODES = SAVE_NODES[1:]

    def __init__(self, XtoY, YtoX, X_feed, Y_feed, X_name='X', Y_name='Y', cycle_lambda=10.0, tb_verbose=True,
                 visualizer=None, learning_rate=2e-4, beta1=0.5, steps=2e5, decay_from=1e5, history=True, graph=None,
                 checkpoints_dir='../../checkpoint', load_model=None):

        self.XtoY = XtoY
        self.YtoX = YtoX

        assert isinstance(XtoY, nets.GAN) and isinstance(YtoX, nets.GAN)
        assert XtoY.in_shape == YtoX.out_shape and XtoY.out_shape == YtoX.in_shape
        assert X_feed.batch_size == Y_feed.batch_size

        self.xybatch_shape = [X_feed.batch_size] + list(XtoY.in_shape)
        self.yxbatch_shape = [Y_feed.batch_size] + list(YtoX.in_shape)

        self.X_name = X_name
        self.Y_name = Y_name
        self.X_feed = X_feed
        self.Y_feed = Y_feed

        self.cycle_lambda = cycle_lambda
        self.tb_verbose = tb_verbose
        self.visualizer = visualizer

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
                self.prev_fake_x = tf.placeholder(tf.float32, shape=self.xybatch_shape)
                self.prev_fake_y = tf.placeholder(tf.float32, shape=self.yxbatch_shape)
            self.cur_x = tf.placeholder(tf.float32, shape=self.xybatch_shape)
            self.cur_y = tf.placeholder(tf.float32, shape=self.yxbatch_shape)

        self.name = None
        self.load_from_ckpt = False
        self.full_checkpoints_dir = None
        self.create_name(checkpoints_dir, load_model)

        logging.info('Cycle GAN instantiated.\n'
                     '%s_shape=' + str(XtoY.in_shape) + '\t%s_shape=' + str(YtoX.in_shape) + '\n'
                                                                                             'cycle_lambda=%f\n'
                                                                                             'learning_rate=%f\tbeta1=%f\tsteps=%d\tdecay_from=%d',
                     X_name, Y_name, cycle_lambda, learning_rate, beta1, steps, decay_from)

    def get_model(self):
        with self.graph.as_default():
            cycle_loss = self._cycle_loss(self.cur_x, self.cur_y, self.cycle_lambda)

            fake_y = self.XtoY.gen(self.cur_x)
            fake_x = self.YtoX.gen(self.cur_y)

            yx_gen_loss = self.YtoX.gen_loss(self.cur_y)
            xy_gen_loss = self.XtoY.gen_loss(self.cur_x)

            xy_gen_weight_loss = self.XtoY.gen.weight_loss()
            yx_gen_weight_loss = self.YtoX.gen.weight_loss()

            y_dis_weight_loss = self.XtoY.dis.weight_loss()
            x_dis_weight_loss = self.YtoX.dis.weight_loss()

            if not isinstance(self.YtoX, nets.WGAN):
                if self.history:
                    x_dis_loss = self.YtoX.dis_loss(self.cur_x, self.prev_fake_x)
                else:
                    x_dis_loss = self.YtoX.dis_loss(self.cur_x, fake_x)
                x_dis_full_loss = x_dis_loss + x_dis_weight_loss
            else:
                if self.history:
                    x_dis_loss = self.YtoX.dis_loss(self.cur_x, self.prev_fake_x)
                    x_dis_grad_loss = self.YtoX.grad_loss(self.cur_x, self.prev_fake_x)
                else:
                    x_dis_loss = self.YtoX.dis_loss(self.cur_x, fake_x)
                    x_dis_grad_loss = self.YtoX.grad_loss(self.cur_x, fake_x)
                x_dis_full_loss = x_dis_loss + x_dis_weight_loss + x_dis_grad_loss

            if not isinstance(self.XtoY, nets.WGAN):
                if self.history:
                    y_dis_loss = self.XtoY.dis_loss(self.cur_y, self.prev_fake_y)
                else:
                    y_dis_loss = self.XtoY.dis_loss(self.cur_y, fake_y)
                y_dis_full_loss = y_dis_loss + y_dis_weight_loss
            else:
                if self.history:
                    y_dis_loss = self.XtoY.dis_loss(self.cur_y, self.prev_fake_y)
                    y_dis_grad_loss = self.XtoY.grad_loss(self.cur_y, self.prev_fake_y)
                else:
                    y_dis_loss = self.XtoY.dis_loss(self.cur_y, fake_y)
                    y_dis_grad_loss = self.XtoY.grad_loss(self.cur_y, fake_y)
                y_dis_full_loss = y_dis_loss + y_dis_weight_loss + y_dis_grad_loss

            xy_selfreg_loss = self.XtoY.selfreg_loss(self.cur_x, fake_y)
            xy_gen_full_loss = xy_gen_loss + xy_gen_weight_loss + cycle_loss + xy_selfreg_loss

            yx_selfreg_loss = self.YtoX.selfreg_loss(self.cur_y, fake_x)
            yx_gen_full_loss = yx_gen_loss + yx_gen_weight_loss + cycle_loss + yx_selfreg_loss

            if self.tb_verbose:
                X_dis_fake = self.YtoX.dis(fake_x)
                X_dis_real = self.YtoX.dis(self.cur_x)
                Y_dis_fake = self.XtoY.dis(fake_y)
                Y_dis_real = self.XtoY.dis(self.cur_y)

                tf.summary.histogram('D_{}/real'.format(self.X_name), X_dis_real)
                tf.summary.histogram('D_{}/fake'.format(self.X_name), X_dis_fake)
                tf.summary.histogram('D_{}/real'.format(self.Y_name), Y_dis_real)
                tf.summary.histogram('D_{}/fake'.format(self.Y_name), Y_dis_fake)

                if self.XtoY.gen.weight_lambda > 0:
                    tf.summary.scalar('{}-{}_gen/weight_loss'.format(self.X_name, self.Y_name), xy_gen_weight_loss)
                if self.XtoY.selfreg_lambda > 0:
                    tf.summary.scalar('{}-{}_gen/selfreg_loss'.format(self.X_name, self.Y_name), xy_selfreg_loss)
                tf.summary.scalar('{}-{}_gen/gen_loss'.format(self.X_name, self.Y_name), xy_gen_loss)
                tf.summary.scalar('{}-{}_gen/full_loss'.format(self.X_name, self.Y_name), xy_gen_full_loss)

                if self.YtoX.gen.weight_lambda > 0:
                    tf.summary.scalar('{}-{}_gen/weight_loss'.format(self.Y_name, self.X_name), yx_gen_weight_loss)
                if self.YtoX.selfreg_lambda > 0:
                    tf.summary.scalar('{}-{}_gen/selfreg_loss'.format(self.Y_name, self.X_name), yx_selfreg_loss)
                tf.summary.scalar('{}-{}_gen/gen_loss'.format(self.Y_name, self.X_name), yx_gen_loss)
                tf.summary.scalar('{}-{}_gen/full_loss'.format(self.Y_name, self.X_name), yx_gen_full_loss)

                if self.YtoX.dis.weight_lambda > 0:
                    tf.summary.scalar('{}_dis/weight_loss'.format(self.X_name), x_dis_weight_loss)
                tf.summary.scalar('{}_dis/dis_loss'.format(self.X_name), x_dis_loss)
                tf.summary.scalar('{}_dis/full_loss'.format(self.X_name), x_dis_full_loss)

                if self.XtoY.dis.weight_lambda > 0:
                    tf.summary.scalar('{}_dis/weight_loss'.format(self.Y_name), y_dis_weight_loss)
                tf.summary.scalar('{}_dis/dis_loss'.format(self.Y_name), y_dis_loss)
                tf.summary.scalar('{}_dis/full_loss'.format(self.Y_name), y_dis_full_loss)

                if isinstance(self.YtoX, nets.WGAN):
                    tf.summary.scalar('{}_dis/grad_loss'.format(self.X_name), x_dis_grad_loss)
                if isinstance(self.XtoY, nets.WGAN):
                    tf.summary.scalar('{}_dis/grad_loss'.format(self.Y_name), y_dis_grad_loss)

                tf.summary.scalar('cycle_loss', cycle_loss)

                if self.visualizer:
                    # this is mine, showing one by one
                    self.visualizer(self.cur_x, fake_y, self.YtoX.gen(fake_y), self.X_name, self.Y_name)
                    self.visualizer(self.cur_y, fake_x, self.XtoY.gen(fake_x), self.Y_name, self.X_name)

            with tf.control_dependencies(tf.get_collection(tf.GraphKeys.UPDATE_OPS)):
                with tf.variable_scope('training', reuse=tf.AUTO_REUSE):
                    global_step = tf.get_variable('global_step',
                                                  shape=(),
                                                  initializer=tf.zeros_initializer)
                yx_gen_opt = self._optimizer(yx_gen_full_loss,
                                             self.YtoX.gen.variables,
                                             global_step,
                                             'Adam/{}-{}_gen'.format(self.Y_name, self.X_name))
                xy_gen_opt = self._optimizer(xy_gen_full_loss,
                                             self.XtoY.gen.variables,
                                             global_step,
                                             'Adam/{}-{}_gen'.format(self.X_name, self.Y_name))
                x_dis_opt = self._optimizer(x_dis_loss,
                                            self.YtoX.dis.variables,
                                            global_step,
                                            'Adam/{}_dis'.format(self.X_name))
                y_dis_opt = self._optimizer(y_dis_loss,
                                            self.XtoY.dis.variables,
                                            global_step,
                                            'Adam/{}_dis'.format(self.Y_name))

                with tf.control_dependencies([xy_gen_opt, yx_gen_opt]):
                    train_gen = tf.no_op('optimizers_gen')
                with tf.control_dependencies([x_dis_opt, y_dis_opt]):
                    train_dis = tf.no_op('optimizers_dis')
                global_step_op = tf.assign_add(global_step, 1)

            logging.info('Created CycleGAN model')

            return {'train': {
                'gen': train_gen,
                'dis': train_dis,
                'global_step': global_step_op
            },
                'losses': {
                    'cycle': cycle_loss,
                    '{}-{}_gen_full'.format(self.X_name, self.Y_name): xy_gen_full_loss,
                    '{}-{}_gen_full'.format(self.Y_name, self.X_name): yx_gen_full_loss,
                    '{}_dis_full'.format(self.X_name): x_dis_full_loss,
                    '{}_dis_full'.format(self.Y_name): y_dis_full_loss,
                },
                'fakes': [fake_x, fake_y]}

    def train(self, gen_train=1, dis_train=1, pool_size=50, log_verbose=True, param_string=None, export_final=True):
        os.makedirs(self.full_checkpoints_dir, exist_ok=True)

        if param_string is not None:
            with open(osp.join(self.full_checkpoints_dir, 'params.flagfile'), 'w') as f:
                f.write(param_string)

        with self.graph.as_default():
            model_ops = self.get_model()
            model_ops['summary'] = tf.summary.merge_all()
            train_writer = tf.summary.FileWriter(self.full_checkpoints_dir, self.graph)
            saver = tf.train.Saver()
            saver_long_term = tf.train.Saver()

        config = tf.ConfigProto()
        config.gpu_options.allow_growth = True
        with tf.Session(graph=self.graph, config=config) as sess:
            if self.load_from_ckpt is not None:
                step = self.load_saved_model(self.full_checkpoints_dir, sess)
            else:
                step = 0
                sess.run(tf.global_variables_initializer())

            if log_verbose:
                varsize_dict = sess.run({
                    '{}->{} gen - num params'.format(self.X_name, self.Y_name): self.XtoY.gen.num_params(),
                    '{}->{} gen - num params'.format(self.Y_name, self.X_name): self.YtoX.gen.num_params(),
                    '{} dis - num params'.format(self.X_name): self.YtoX.dis.num_params(),
                    '{} dis - num params'.format(self.Y_name): self.YtoX.dis.num_params(),
                })
                for k, v in varsize_dict.items():
                    logging.info('\t{}:\t{}'.format(k, v))

            if self.history:
                x_pool = utils.DataBuffer(pool_size, self.X_feed.batch_size)
                y_pool = utils.DataBuffer(pool_size, self.Y_feed.batch_size)

            # try:
            if self.history:
                cur_x, cur_y = sess.run([self.X_feed.feed(), self.Y_feed.feed()])
            while step < self.steps:
                if self.history:
                    fx, fy = sess.run(model_ops['fakes'], feed_dict={
                        self.cur_x: cur_x,
                        self.cur_y: cur_y,
                    })
                cur_x, cur_y = sess.run([self.X_feed.feed(), self.Y_feed.feed()])
                if self.history:
                    feeder_dict = {
                        self.cur_x: cur_x,
                        self.cur_y: cur_y,
                        self.prev_fake_x: x_pool.query(fx, step),
                        self.prev_fake_y: y_pool.query(fy, step),
                    }
                else:
                    feeder_dict = {
                        self.cur_x: cur_x,
                        self.cur_y: cur_y,
                    }
                for _ in range(dis_train):
                    sess.run(model_ops['train']['dis'], feed_dict=feeder_dict)
                for _ in range(gen_train):
                    # todo, debug why it fails sometimes with dynamic input, using the self.XtoY.gen.layers_dict to see things in problematic layer
                    # sess.run(self.XtoY.gen.layers_dicts[1]['c-7-1-3-t'], feed_dict={self.cur_x: cur_x}) works
                    # then uncomment the try-catch and probably get rid of all the coords things
                    sess.run(model_ops['train']['gen'], feed_dict=feeder_dict)

                # michal nastaveni: každých 2500 logovat trénovací, každých 25000 validační a ukládat model
                # every 100 steps mean cca every minute it is logged on 1080Ti
                if step % 100 == 0:
                    summary, _ = sess.run([model_ops['summary'], model_ops['train']['global_step']],
                                          feed_dict=feeder_dict)
                    train_writer.add_summary(summary, step)
                    train_writer.flush()

                if log_verbose and step % 100 == 0:
                    logging.info('------ Step {:d} ------'.format(step))
                    losses = sess.run(model_ops['losses'], feed_dict=feeder_dict)
                    for k, v in losses.items():
                        logging.info('\t{}:\t{:8f}'.format(k, v))

                # every 5000 means cca every hour and 15 minutes model is saved on 1080Ti
                if step % 5000 == 0:
                    save_path = saver.save(sess, osp.join(self.full_checkpoints_dir, 'model.ckpt'), global_step=step)
                    logging.info('Model saved in file: %s', save_path)

                if step % 50000 == 0:
                    save_path = saver_long_term.save(
                        sess, osp.join(self.full_checkpoints_dir, 'model-long-term.ckpt'), global_step=step,
                        latest_filename='checkpoint-long-term')
                    logging.info('Long term model saved in file: %s', save_path)

                step += 1

            logging.info('Stopping after %d iterations', self.steps)
            save_path = saver.save(sess, osp.join(self.full_checkpoints_dir, 'model.ckpt'), global_step=step)
            if log_verbose:
                logging.info('Model saved in file: %s', save_path)
            if export_final:
                self.export(sess, self.full_checkpoints_dir)

    @staticmethod
    def load_saved_model(full_checkpoints_dir, sess):
        checkpoint = tf.train.get_checkpoint_state(full_checkpoints_dir)
        if checkpoint is None:
            step = 0
            sess.run(tf.global_variables_initializer())
        else:
            meta_graph_path = checkpoint.model_checkpoint_path + '.meta'
            restore = tf.train.import_meta_graph(meta_graph_path)
            restore.restore(sess, tf.train.latest_checkpoint(full_checkpoints_dir))
            step = int(osp.basename(meta_graph_path).split('-')[1].split('.')[0])
        return step

    def export(self, sess, export_dir):
        is_training = self.XtoY.gen.is_training
        self.XtoY.gen.is_training = self.XtoY.dis.is_training = False
        self.YtoX.gen.is_training = self.YtoX.dis.is_training = False
        self._export_one_part(sess, export_dir, True, '{}2{}.pb'.format(self.X_name, self.Y_name))
        self._export_one_part(sess, export_dir, False, '{}2{}.pb'.format(self.Y_name, self.X_name))
        self.XtoY.gen.is_training = self.XtoY.dis.is_training = is_training
        self.YtoX.gen.is_training = self.YtoX.dis.is_training = is_training

    def _export_one_part(self, sess, export_dir, XtoY, model_name):
        with self.graph.as_default():
            normer = self.X_feed.normalize if XtoY else self.Y_feed.normalize
            denormer = self.Y_feed.denormalize if XtoY else self.X_feed.denormalize
            data_in = tf.expand_dims(normer(tf.placeholder(tf.float32,
                                                           shape=self.XtoY.in_shape if XtoY else self.YtoX.out_shape,
                                                           name='input')), 0)
            out = self.XtoY.gen(data_in) if XtoY else self.YtoX.gen(data_in)
            d_in = self.XtoY.dis(data_in) if XtoY else self.YtoX.dis(data_in)
            d_out = self.YtoX.dis(out) if XtoY else self.XtoY.dis(out)
            denormer(tf.squeeze(out, axis=0), name='output')
            tf.reduce_mean(d_in, name='d_input')
            tf.reduce_mean(d_out, name='d_output')

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
        step_1 = CycleGAN._export_cp_one_part(XtoY.gen, XtoY.dis, YtoX.dis,
                                              X_normer, Y_denormer,
                                              XtoY.in_shape,
                                              checkpoint_dir, export_dir, '{}2{}.pb'.format(X_name, Y_name))
        step_2 = CycleGAN._export_cp_one_part(YtoX.gen, YtoX.dis, XtoY.dis,
                                              Y_normer, X_denormer,
                                              YtoX.in_shape,
                                              checkpoint_dir, export_dir, '{}2{}.pb'.format(Y_name, X_name))
        assert step_1 == step_2
        return step_1

    @staticmethod
    def _export_cp_one_part(gen,
                            out_dis, in_dis, normer, denormer,
                            in_shape,
                            cp_dir, export_dir, model_name):
        graph = tf.Graph()
        with graph.as_default():
            # data_in = tf.expand_dims(normer(tf.placeholder(tf.float32,
            #                                                shape=in_shape,
            #                                                name='input')), 0)
            # above is for one image per batch, this is for any size batch
            data_in = normer(tf.placeholder(tf.float32,
                                            shape=[None] + list(in_shape),
                                            name='input'))
            out = gen(data_in)
            d_in = in_dis(data_in)
            d_out = out_dis(out)
            # denormer(tf.squeeze(out, axis=0), name='output')
            # above is for one image per batch, this is for any size batch
            denormer(out, name='output')
            tf.reduce_mean(d_in, name='d_input')
            tf.reduce_mean(d_out, name='d_output')
            restore = tf.train.Saver()

        # no need to use GPU for export
        config = tf.ConfigProto(device_count={'GPU': 0})
        config.gpu_options.allow_growth = True
        with tf.Session(graph=graph, config=config) as sess:
            sess.run(tf.global_variables_initializer())
            cp = tf.train.latest_checkpoint(cp_dir)
            restore.restore(sess, cp)
            output_graph_def = gu.extract_sub_graph(
                gu.remove_training_nodes(
                    gu.convert_variables_to_constants(
                        sess, graph.as_graph_def(), CycleGAN.SAVE_NODES),
                    CycleGAN.SAVE_NODES),
                CycleGAN.SAVE_NODES)

            # just getting the current step
            meta_graph_path = cp + '.meta'
            step = str(osp.basename(meta_graph_path).split('-')[1].split('.')[0])

            tf.train.write_graph(output_graph_def, osp.join(export_dir, step), model_name, as_text=False)
        return step

    @staticmethod
    def test_one_part(pb_model, dataset, data_size, batch_size, postprocessing=lambda a, b, c: a):
        graph = tf.get_default_graph()
        d_input, d_output, input_var, output = CycleGAN.get_graph_outputs(graph, dataset[0], pb_model)
        iteration_num = tf.placeholder(tf.int32)
        postprocess = postprocessing(output, dataset[1], dataset[2])

        config = tf.ConfigProto()
        config.gpu_options.allow_growth = True
        with tf.Session(graph=graph, config=config) as sess:
            sess.run(tf.global_variables_initializer())

            widgets = [progressbar.Percentage(), ' ', progressbar.Counter(), ' ', progressbar.Bar(), ' ',
                       progressbar.FileTransferSpeed()]
            pbar = progressbar.ProgressBar(widgets=widgets, max_value=data_size * batch_size).start()
            i = 0
            while True:
                try:
                    pbar.update(i * batch_size)
                    i += 1
                    out, din, dout, _ = sess.run([output, d_input, d_output, postprocess],
                                                 feed_dict={iteration_num: i})
                except tf.errors.OutOfRangeError:
                    break

    @staticmethod
    def get_graph_outputs(graph, input_var, pb_model):
        with graph.as_default():
            graph_def = tf.GraphDef()
            with tf.gfile.FastGFile(pb_model, 'rb') as model_file:
                graph_def.ParseFromString(model_file.read())
            output, d_input, d_output = list(
                map(lambda x: x.outputs[0],
                    tf.import_graph_def(graph_def,
                                        input_map={'input': input_var},
                                        return_elements=CycleGAN.OUTPUT_NODES)))
        return d_input, d_output, input_var, output

    @staticmethod
    def save_output(all_data, d_inputs, d_outputs, include_input, outfile, outputs):
        kwarg_map = {'output': np.array(outputs),
                     'd_input': np.array(d_inputs),
                     'd_output': np.array(d_outputs)
                     }
        if include_input:
            kwarg_map['input'] = all_data
        if osp.exists(outfile):
            logging.warning('File %s exists, it will be overwritten', outfile)
        np.savez_compressed(outfile, **kwarg_map)
        logging.info('Saved file %s', outfile)

    def _optimizer(self, loss, variables, global_step, name):
        with tf.variable_scope(name):
            learning_rate = tf.where(tf.greater_equal(global_step, self.decay_from),
                                     tf.train.polynomial_decay(self.learning_rate,
                                                               global_step - self.decay_from,
                                                               self.steps - self.decay_from,
                                                               0, power=1.0),
                                     self.learning_rate)

            if self.tb_verbose:
                tf.summary.scalar('learning_rate', learning_rate)

            adam = tf.train.AdamOptimizer(learning_rate, beta1=self.beta1, name=name)
            learning_step = adam.minimize(loss, var_list=variables)
            return learning_step

    def _cycle_loss(self, x, y, cycle_lambda):
        y_to_y_diff = tf.abs(self.XtoY.gen(self.YtoX.gen(y)) - y)
        x_to_x_diff = tf.abs(self.YtoX.gen(self.XtoY.gen(x)) - x)
        return cycle_lambda * (tf.reduce_mean(x_to_x_diff) + tf.reduce_mean(y_to_y_diff))

    def create_name(self, checkpoints_dir, load_model):
        if load_model is not None:
            self.name = load_model
            full_checkpoints_dir = osp.join(checkpoints_dir, self.name)
            self.load_from_ckpt = True
        else:
            name = datetime.now().strftime('%Y-%m-%d--%H-%M')

            i = 0
            name_try = name + '-' + str(i)
            while osp.exists(osp.join(checkpoints_dir, name_try)):
                i += 1
                name_try = name + '-' + str(i)
            self.name = name_try
            full_checkpoints_dir = osp.join(checkpoints_dir, self.name)
            self.load_from_ckpt = False
        self.full_checkpoints_dir = full_checkpoints_dir
