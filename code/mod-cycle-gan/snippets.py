import logging
logging.basicConfig(level=logging.INFO)
import cycle
import cycle.models.lidar as lidar
valeotfr = cycle.utils.TFReader('/datagrid/personal/jasekota/dip-dataset/valeo/valeo-trn.tfrecords', 'valeo', lidar.DATA_SHAPE, normer=lidar.utils.normer, denormer=lidar.utils.denormer)
gtatfr = cycle.utils.TFReader('/datagrid/personal/jasekota/dip-dataset/gta/gta-trn.tfrecords', 'gta', lidar.DATA_SHAPE, normer=lidar.utils.normer, denormer=lidar.utils.denormer)
valeogan = cycle.nets.WGAN(lidar.Generator('valeo-gen', True, 2), lidar.Discriminator('valeo-dis', True, 2), lidar.DATA_SHAPE, lidar.DATA_SHAPE, 2, 2, 5)
gtagan = cycle.nets.WGAN(lidar.Generator('gta-gen', True, 2), lidar.Discriminator('gta-dis', True, 2), lidar.DATA_SHAPE, lidar.DATA_SHAPE, 2, 2, 5)
cygan = cycle.CycleGAN(valeogan, gtagan, 'valeo', 'gta', valeotfr, gtatfr)
cygan.train('/datagrid/personal/jasekota/cycle-dip/cp/', 5, 1)



import cycle
import tensorflow as tf
datafile = '/datagrid/personal/jasekota/dip-dataset/valeo/valeo-tst.npy'
import numpy as np
all_data = np.load(datafile)
modelpb = '/datagrid/personal/jasekota/cycle-dip/cp/20180423-1005-0/valeo2gta.pb'
graph = tf.Graph()
with graph.as_default():
	with tf.gfile.FastGFile(modelpb, 'rb') as mfile:
		graphdef = tf.GraphDef()
		graphdef.ParseFromString(mfile.read())
	input = tf.placeholder(tf.float32, shape=cycle.models.lidar.DATA_SHAPE)
	output, dinput, doutput = tf.import_graph_def(graphdef, input_map={'input': input}, return_elements=['output:0', 'd_input:0', 'd_output:0'])

outs = []
douts = []
dins = []
with tf.Session(graph=graph) as sess:
	sess.run(tf.global_variables_initializer())
	for data in all_data:
		gout, gdin, gdout = sess.run([output, dinput, doutput], feed_dict={input:data})
		outs.append(gout)
		dins.append(gdin)
		douts.append(gdout)

outs = np.array(outs)
dins = np.array(dins)
douts = np.array(douts)
np.savez_compressed('/datagrid/personal/jasekota/dip-dataset/valeo2gta.npz', input=all_data, output=outs, d_input=dins, d_output=douts)
