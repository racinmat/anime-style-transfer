#!/usr/bin/env python3

import tensorflow as tf
import glob
import sys
import os

replacers = [('valeotogta_gen', 'valeo-gta_gen'), ('gtatovaleo_gen', 'gta-valeo_gen'), ('gta_gen', 'valeo-gta_gen'), ('valeo_gen', 'gta-valeo_gen')]

class cd:
	def __init__(self, newPath):
		self.newPath = os.path.expanduser(newPath)

	def __enter__(self):
		self.savedPath = os.getcwd()
		os.chdir(self.newPath)

	def __exit__(self, etype, value, traceback):
		os.chdir(self.savedPath)

def modifytag(tag):
	for rr in replacers:
		tag = tag.replace(*rr)
	if 'learning_rate' in tag:
		tag = tag[:tag.find('learning_rate')+len('learning_rate')]
	return tag

if __name__ == '__main__':
	with cd(sys.argv[1]):
		f = sorted(glob.glob('events*'))
		sum = tf.train.summary_iterator(f[0])
		fw = tf.summary.FileWriter('.')

		for ev in sum:
			if ev.summary.value:
				for i, val in enumerate(ev.summary.value):
					ev.summary.value[i].tag = modifytag(ev.summary.value[i].tag)
			fw.add_event(ev)

		fw.close()
