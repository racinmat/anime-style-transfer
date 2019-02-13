#!/usr/bin/env python3

import os
import os.path as osp
import logging
from datetime import datetime

import progressbar
import tensorflow as tf
from tensorflow import graph_util as gu
import numpy as np
from tensorflow.contrib.gan.python.features import tensor_pool
from tensorflow.python.framework import ops
from tensorflow.python.ops import clip_ops
from cycle.nets import GAN, BaseNet
from cycle.utils import TFReader
from . import utils, nets


class CycleGAN:
    SAVE_NODES = ['input', 'output', 'd_input', 'd_output']
    OUTPUT_NODES = SAVE_NODES[1:]

    def __init__(self, XtoY: GAN, YtoX: GAN, X_feed: TFReader, Y_feed: TFReader, X_name='X', Y_name='Y',
                 cycle_lambda=10.0, cycle_lambda_end=10.0, tb_verbose=True, visualizer=None, learning_rate=2e-4,
                 beta1=0.5, steps=2e5, decay_from=1e5, graph=None, checkpoints_dir='../../checkpoint', load_model=None):

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
        self.cycle_lambda_end = cycle_lambda_end

        self.tb_verbose = tb_verbose
        self.visualizer = visualizer

        self.learning_rate = learning_rate
        self.beta1 = beta1
        self.steps = steps
        self.decay_from = decay_from

        if graph is None:
            self.graph = tf.get_default_graph()
        else:
            self.graph = graph

        self.cur_x = None
        self.cur_y = None

        with self.graph.as_default():
            self.build_placeholders()

        self.name = None
        self.load_from_ckpt = False
        self.full_checkpoints_dir = None
        self.create_name(checkpoints_dir, load_model)

        self.train_writer = None

        logging.info('Cycle GAN instantiated.\n'
                     '%s_shape=' + str(XtoY.in_shape) + '\t%s_shape=' + str(YtoX.in_shape) + '\n' +
                     'cycle_lambda=%f\n'
                     'learning_rate=%f\tbeta1=%f\tsteps=%d\tdecay_from=%d',
                     X_name, Y_name, cycle_lambda, learning_rate, beta1, steps, decay_from)

    def build_placeholders(self):
        self.cur_x = tf.placeholder(tf.float32, shape=self.xybatch_shape, name='gt_{}'.format(self.X_name))
        self.cur_y = tf.placeholder(tf.float32, shape=self.yxbatch_shape, name='gt_{}'.format(self.Y_name))
        # for simple debugging, so I have data
        # self.cur_x = self.X_feed.feed()
        # self.cur_y = self.Y_feed.feed()

    def get_model(self):
        with self.graph.as_default():

            fake_y = self.XtoY.gen.build(self.cur_x)
            fake_x = self.YtoX.gen.build(self.cur_y)

            X_dis_fake = self.YtoX.dis.build(fake_x)
            X_dis_real = self.YtoX.dis.build(self.cur_x)
            Y_dis_fake = self.XtoY.dis.build(fake_y)
            Y_dis_real = self.XtoY.dis.build(self.cur_y)

            with tf.variable_scope('training', reuse=tf.AUTO_REUSE):
                global_step = tf.get_variable('global_step', shape=(), initializer=tf.zeros_initializer)

            self.build_fake_pool(global_step, fake_x, fake_y)

            with tf.name_scope('cycle-loss'):
                cycle_loss = self._cycle_loss(self.cur_x, self.cur_y, global_step)
            with tf.name_scope('{}-{}-gen-loss'.format(self.Y_name, self.X_name)):
                yx_gen_loss = self.YtoX.gen_loss(self.cur_y)
                yx_gen_weight_loss = self.YtoX.gen.weight_loss()
                yx_selfreg_loss = self.YtoX.selfreg_loss(self.cur_y, fake_x)
                yx_gen_full_loss = yx_gen_loss + yx_gen_weight_loss + cycle_loss + yx_selfreg_loss
            with tf.name_scope('{}-{}-gen-loss'.format(self.X_name, self.Y_name)):
                xy_gen_loss = self.XtoY.gen_loss(self.cur_x)
                xy_gen_weight_loss = self.XtoY.gen.weight_loss()
                xy_selfreg_loss = self.XtoY.selfreg_loss(self.cur_x, fake_y)
                xy_gen_full_loss = xy_gen_loss + xy_gen_weight_loss + cycle_loss + xy_selfreg_loss

            x_dis_full_loss, y_dis_full_loss = self.build_dis_losses(fake_x, fake_y)

            # update ops are from batch normalization
            with tf.control_dependencies(tf.get_collection(tf.GraphKeys.UPDATE_OPS)):
                yx_gen_opt = self._optimizer(yx_gen_full_loss, self.YtoX.gen.variables, global_step,
                                             'Adam/{}-{}_gen'.format(self.Y_name, self.X_name))
                xy_gen_opt = self._optimizer(xy_gen_full_loss, self.XtoY.gen.variables, global_step,
                                             'Adam/{}-{}_gen'.format(self.X_name, self.Y_name))
                x_dis_opt = self._optimizer(x_dis_full_loss, self.YtoX.dis.variables, global_step,
                                            'Adam/{}_dis'.format(self.X_name))
                y_dis_opt = self._optimizer(y_dis_full_loss, self.XtoY.dis.variables, global_step,
                                            'Adam/{}_dis'.format(self.Y_name))

            with tf.control_dependencies([xy_gen_opt, yx_gen_opt]):
                train_gen = tf.no_op('optimizers_gen')
            with tf.control_dependencies([x_dis_opt, y_dis_opt]):
                train_dis = tf.no_op('optimizers_dis')

            # linearize.linearize([train_gen, train_dis])   # does not work

            tf.summary.scalar('global_step', global_step)
            global_step_op = tf.assign_add(global_step, 1)

            if self.tb_verbose:
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

                tf.summary.scalar('cycle_loss', cycle_loss)

                for var in tf.trainable_variables():
                    tf.summary.histogram(var.op.name, var)

                if self.visualizer:
                    # this is mine, showing one by one
                    self.visualizer(self.cur_x, fake_y, self.YtoX.gen.build(fake_y), self.X_name, self.Y_name)
                    self.visualizer(self.cur_y, fake_x, self.XtoY.gen.build(fake_x), self.Y_name, self.X_name)

            logging.info('Created CycleGAN model')

            return {
                'train': {
                    'gen': train_gen,
                    'dis': train_dis,
                    'global_step': global_step_op,  # manual global step allows me to use multiple sess.run per step
                },
                'losses': {
                    'cycle': cycle_loss,
                    '{}-{}_gen_full'.format(self.X_name, self.Y_name): xy_gen_full_loss,
                    '{}-{}_gen_full'.format(self.Y_name, self.X_name): yx_gen_full_loss,
                    '{}_dis_full'.format(self.X_name): x_dis_full_loss,
                    '{}_dis_full'.format(self.Y_name): y_dis_full_loss,
                },
                'fakes': {
                    'x': fake_x,
                    'y': fake_y,
                },
            }

    def build_dis_losses(self, fake_x, fake_y):
        x_dis_full_loss = self.YtoX.construct_dis_full_loss(self.cur_x, fake_x, self.X_name)
        y_dis_full_loss = self.XtoY.construct_dis_full_loss(self.cur_y, fake_y, self.Y_name)
        return x_dis_full_loss, y_dis_full_loss

    def train(self, gen_train=1, dis_train=1, log_verbose=True, param_string=None, export_final=True):
        os.makedirs(self.full_checkpoints_dir, exist_ok=True)

        if param_string is not None:
            with open(osp.join(self.full_checkpoints_dir, 'params.flagfile'), 'w') as f:
                f.write(param_string)

        with self.graph.as_default():
            model_ops = self.get_model()
            model_ops['summary'] = tf.summary.merge_all()
            self.train_writer = tf.summary.FileWriter(self.full_checkpoints_dir, self.graph)
            saver = tf.train.Saver()
            saver_long_term = tf.train.Saver(max_to_keep=None)

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

            while step < self.steps:
                feeder_dict = self.base_train_iteration(dis_train, gen_train, model_ops, sess, step)

                # michal nastaveni: každých 2500 logovat trénovací, každých 25000 validační a ukládat model
                if step % 250 == 0:
                    # full tracing for tensorboard debugging
                    # on windows, path to some dll is missing, you can fix it by
                    # https://github.com/tensorflow/tensorflow/issues/6235
                    # run_options = tf.RunOptions(trace_level=tf.RunOptions.FULL_TRACE)
                    # run_metadata = tf.RunMetadata()
                    # summary, losses, _ = sess.run([model_ops['summary'], model_ops['losses'], model_ops['train']],
                    #                               feed_dict=feeder_dict, options=run_options,
                    #                               run_metadata=run_metadata)

                    summary, losses = sess.run([model_ops['summary'], model_ops['losses']], feed_dict=feeder_dict)
                    self.train_writer.add_summary(summary, step)
                    # self.train_writer.add_run_metadata(run_metadata, 'step%d' % step)
                    self.train_writer.flush()

                    if log_verbose:
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

    def base_train_iteration(self, dis_train, gen_train, model_ops, sess, step):
        cur_x, cur_y, _ = sess.run([self.X_feed.feed(), self.Y_feed.feed(), model_ops['train']['global_step']])
        feeder_dict = {
            self.cur_x: cur_x,
            self.cur_y: cur_y,
        }

        for _ in range(gen_train):
            sess.run(model_ops['train']['gen'], feed_dict=feeder_dict)
        for _ in range(dis_train):
            sess.run(model_ops['train']['dis'], feed_dict=feeder_dict)
        return feeder_dict

    def build_fake_pool(self, global_step, fake_x, fake_y):
        pass

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
            step = int(CycleGAN.meta_path_to_step(meta_graph_path))
        return step

    @staticmethod
    def meta_path_to_step(meta_graph_path):
        name_ext = osp.basename(meta_graph_path).split('-')[1]  # type: str
        return name_ext.split('.')[0]

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
            out = self.XtoY.gen.build(data_in) if XtoY else self.YtoX.gen.build(data_in)
            d_in = self.XtoY.dis.build(data_in) if XtoY else self.YtoX.dis.build(data_in)
            d_out = self.YtoX.dis.build(out) if XtoY else self.XtoY.dis.build(out)
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
    def _export_cp_one_part(gen: BaseNet, out_dis: BaseNet, in_dis: BaseNet, normer, denormer, in_shape, cp_dir,
                            export_dir, model_name):
        graph = tf.Graph()
        with graph.as_default():
            # data_in = tf.expand_dims(normer(tf.placeholder(tf.float32,
            #                                                shape=in_shape,
            #                                                name='input')), 0)
            # above is for one image per batch, this is for any size batch
            data_in = normer(tf.placeholder(tf.float32,
                                            shape=[None] + list(in_shape),
                                            name='input'))
            out = gen.build(data_in)
            d_in = in_dis.build(data_in)
            d_out = out_dis.build(out)
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
            step = str(CycleGAN.meta_path_to_step(meta_graph_path))

            tf.train.write_graph(output_graph_def, osp.join(export_dir, step), model_name, as_text=False)
        return step

    @staticmethod
    def test_one_part(pb_model, dataset, data_size, batch_size, postprocessing=lambda a, b, c: a):
        graph = tf.get_default_graph()
        d_input, d_output, output = CycleGAN.get_graph_outputs(graph, dataset[0], pb_model)
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
        return d_input, d_output, output

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

    def compute_adam_gradients(self, adam: tf.train.Optimizer, loss, variables):
        from tensorflow.python.training.optimizer import Optimizer
        from tensorflow.python.eager import context
        from tensorflow.python.framework import dtypes
        from tensorflow.python.ops import control_flow_ops
        from tensorflow.python.ops import variable_scope
        from tensorflow.python.training import distribute as distribute_lib
        from tensorflow.python.training import distribution_strategy_context
        from tensorflow.python.util import nest

        def compute_gradients(optimizer, loss, var_list=None,
                              gate_gradients=Optimizer.GATE_OP,
                              aggregation_method=None,
                              colocate_gradients_with_ops=False,
                              grad_loss=None):
            if callable(loss):
                from tensorflow.python.eager import backprop
                with backprop.GradientTape() as tape:
                    if var_list is not None:
                        tape.watch(var_list)
                    loss_value = loss()

                    # Scale loss if using a "mean" loss reduction and multiple towers.
                    # Have to be careful to call distribute_lib.get_loss_reduction()
                    # *after* loss() is evaluated, so we know what loss reduction it uses.
                    # TODO(josh11b): Test that we handle weight decay in a reasonable way.
                    if (distribute_lib.get_loss_reduction() ==
                            variable_scope.VariableAggregation.MEAN):
                        num_towers = distribution_strategy_context.get_distribution_strategy(
                        ).num_towers
                        if num_towers > 1:
                            loss_value *= (1. / num_towers)

                if var_list is None:
                    var_list = tape.watched_variables()
                # TODO(jhseu): Figure out why GradientTape's gradients don't require loss
                # to be executed.
                with ops.control_dependencies([loss_value]):
                    grads = tape.gradient(loss_value, var_list, grad_loss)
                return list(zip(grads, var_list))

            # Non-callable/Tensor loss case
            if context.executing_eagerly():
                raise RuntimeError(
                    "`loss` passed to Optimizer.compute_gradients should "
                    "be a function when eager execution is enabled.")

            # Scale loss if using a "mean" loss reduction and multiple towers.
            if (distribute_lib.get_loss_reduction() ==
                    variable_scope.VariableAggregation.MEAN):
                num_towers = distribution_strategy_context.get_distribution_strategy(
                ).num_towers
                if num_towers > 1:
                    loss *= (1. / num_towers)

            if gate_gradients not in [Optimizer.GATE_NONE, Optimizer.GATE_OP,
                                      Optimizer.GATE_GRAPH]:
                raise ValueError("gate_gradients must be one of: Optimizer.GATE_NONE, "
                                 "Optimizer.GATE_OP, Optimizer.GATE_GRAPH.  Not %s" %
                                 gate_gradients)
            optimizer._assert_valid_dtypes([loss])
            if grad_loss is not None:
                optimizer._assert_valid_dtypes([grad_loss])
            if var_list is None:
                var_list = (
                        variables.trainable_variables() +
                        ops.get_collection(ops.GraphKeys.TRAINABLE_RESOURCE_VARIABLES))
            else:
                var_list = nest.flatten(var_list)
            # pylint: disable=protected-access
            var_list += ops.get_collection(ops.GraphKeys._STREAMING_MODEL_PORTS)
            # pylint: enable=protected-access
            from tensorflow.python.training.optimizer import _get_processor
            processors = [_get_processor(v) for v in var_list]
            if not var_list:
                raise ValueError("No variables to optimize.")
            var_refs = [p.target() for p in processors]
            # original gradients computation
            # grads = tf.gradients(
            #     loss, var_refs, grad_ys=grad_loss,
            #     gate_gradients=(gate_gradients == Optimizer.GATE_OP),
            #     aggregation_method=aggregation_method,
            #     colocate_gradients_with_ops=colocate_gradients_with_ops)
            # using gradient check-pointing
            from memory_saving_gradients import gradients
            # setting outputs of different networks
            tensors_to_checkpoint = self.get_tensors_to_checkpoint()

            # just specifying memory as parameter fails
            grads = gradients(
                loss, var_refs, grad_ys=grad_loss,
                gate_gradients=(gate_gradients == Optimizer.GATE_OP),
                aggregation_method=aggregation_method,
                colocate_gradients_with_ops=colocate_gradients_with_ops,
                checkpoints='speed')

            if gate_gradients == Optimizer.GATE_GRAPH:
                grads = control_flow_ops.tuple(grads)
            grads_and_vars = list(zip(grads, var_list))
            optimizer._assert_valid_dtypes(
                [v for g, v in grads_and_vars
                 if g is not None and v.dtype != dtypes.resource])
            return grads_and_vars

        # just copied so I can change gradients
        # computed_gradients = compute_gradients(adam, loss, var_list=variables)

        computed_gradients = adam.compute_gradients(loss, var_list=variables)  # original gradient
        return computed_gradients

    def get_tensors_to_checkpoint(self):
        tensors_to_checkpoint = [
            self.XtoY.gen.build(self.cur_x), self.YtoX.gen.build(self.cur_y),
            self.XtoY.gen.build(self.YtoX.gen.build(self.cur_y)), self.YtoX.gen.build(self.XtoY.gen.build(self.cur_x)),
            self.XtoY.dis.build(self.XtoY.gen.build(self.cur_x)), self.YtoX.dis.build(self.YtoX.gen.build(self.cur_x)),
            self.XtoY.dis.build(self.cur_y), self.YtoX.dis.build(self.cur_x),
        ]
        return tensors_to_checkpoint

    def _optimizer(self, loss, variables, global_step, name):
        with tf.variable_scope(name):
            learning_rate = tf.where(
                tf.greater_equal(global_step, self.decay_from),
                tf.train.polynomial_decay(self.learning_rate, global_step - self.decay_from,
                                          decay_steps=self.steps - self.decay_from, end_learning_rate=0, power=1.0),
                self.learning_rate)

            if self.tb_verbose:
                tf.summary.scalar('learning_rate', learning_rate)

            adam = tf.train.AdamOptimizer(learning_rate, beta1=self.beta1, name=name)

            # this part is basically copied from tensorflow.python.training.optimizer.Optimizer#minimize
            # to access gradients
            gradients = self.compute_adam_gradients(adam, loss, variables)
            tf.summary.scalar("gradient_norm/global", clip_ops.global_norm(list(zip(*gradients))[0]))

            # Add histograms for variables, gradients and gradient norms
            # copied from tensorflow.contrib.layers.python.layers.optimizers.optimize_loss
            for gradient, variable in gradients:
                if isinstance(gradient, ops.IndexedSlices):
                    grad_values = gradient.values
                else:
                    grad_values = gradient

                if grad_values is not None:
                    var_name = variable.name.replace(":", "_")
                    tf.summary.histogram("gradients/%s" % var_name, grad_values)
                    tf.summary.scalar("gradient_norm/%s" % var_name, clip_ops.global_norm([grad_values]))

            # I increment global step myself because applying gradiesnts is done in multiple session run
            # Interesting note: global step was incremented once per each optimizer, so 5 times faster
            learning_step = adam.apply_gradients(gradients)

            return learning_step

    def _cycle_loss(self, x, y, global_step):
        y_to_y_diff = tf.abs(self.XtoY.gen.build(self.YtoX.gen.build(y)) - y)
        x_to_x_diff = tf.abs(self.YtoX.gen.build(self.XtoY.gen.build(x)) - x)

        curr_lambda = tf.train.polynomial_decay(
            self.cycle_lambda, global_step, decay_steps=self.steps, end_learning_rate=self.cycle_lambda_end, power=1.0)

        return curr_lambda * (tf.reduce_mean(x_to_x_diff) + tf.reduce_mean(y_to_y_diff))

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


class HistoryCycleGAN(CycleGAN):
    def __init__(self, XtoY: GAN, YtoX: GAN, X_feed, Y_feed, X_name='X', Y_name='Y', cycle_lambda=10.0,
                 cycle_lambda_end=10.0, tb_verbose=True, visualizer=None, learning_rate=2e-4, beta1=0.5, steps=2e5,
                 decay_from=1e5, graph=None, checkpoints_dir='../../checkpoint', load_model=None, pool_size=50):
        self.prev_fake_x = None
        self.prev_fake_y = None
        self.x_pool = None
        self.y_pool = None
        self.pool_size = pool_size
        super().__init__(
            XtoY, YtoX, X_feed, Y_feed, X_name, Y_name, cycle_lambda, cycle_lambda_end, tb_verbose, visualizer,
            learning_rate, beta1, steps, decay_from, graph, checkpoints_dir, load_model)

    def build_placeholders(self):
        # old way
        name = 'prev_fake_{}'
        self.prev_fake_x = tf.placeholder(tf.float32, shape=self.xybatch_shape, name=name.format(self.X_name))
        self.prev_fake_y = tf.placeholder(tf.float32, shape=self.yxbatch_shape, name=name.format(self.Y_name))
        super().build_placeholders()

    def build_dis_losses(self, fake_x, fake_y):
        return super().build_dis_losses(self.prev_fake_x, self.prev_fake_y)

    def build_fake_pool(self, global_step, fake_x, fake_y):
        # tried the tensor_pool in tfgan contrib, but it's slower
        self.x_pool = utils.DataBuffer(self.pool_size, self.X_feed.batch_size)
        self.y_pool = utils.DataBuffer(self.pool_size, self.Y_feed.batch_size)

    def get_tensors_to_checkpoint(self):
        tensors_to_checkpoint = super().get_tensors_to_checkpoint()
        tensors_to_checkpoint += [
            self.XtoY.dis.build(self.prev_fake_y), self.YtoX.dis.build(self.prev_fake_x),
        ]
        return tensors_to_checkpoint

    def base_train_iteration(self, dis_train, gen_train, model_ops, sess, step):
        cur_x, cur_y, _ = sess.run([self.X_feed.feed(), self.Y_feed.feed(), model_ops['train']['global_step']])

        feeder_dict = {
            self.cur_x: cur_x,
            self.cur_y: cur_y,
        }
        for _ in range(gen_train):
            _, fakes = sess.run([model_ops['train']['gen'], model_ops['fakes']], feed_dict=feeder_dict)
            fx, fy = fakes['x'], fakes['y']
            feeder_dict = {
                self.cur_x: cur_x,
                self.cur_y: cur_y,
                self.prev_fake_x: self.x_pool.query(fx, step),
                self.prev_fake_y: self.y_pool.query(fy, step),
            }
        for _ in range(dis_train):
            sess.run(model_ops['train']['dis'], feed_dict=feeder_dict)
        return feeder_dict
