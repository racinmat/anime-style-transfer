#!/usr/bin/env python3

import numpy as np
from PIL import Image
import os
import os.path as osp
import sys
import rays

scale = 255.0/131.0

def load_data(filename, limit=100):
	arr = np.load(filename)
	input = arr['input'][:limit].copy()
	output = arr['output'][:limit].copy()
	del arr
	return input, output


def process_one_example(i, orig, conv, path):
	Image.fromarray(((orig[:,:,0] * (orig[:,:,2] > 0.5)) * scale).astype('u1')).save(osp.join(path, '%03d.orig.png' % (i,)), 'PNG')
	Image.fromarray(((conv[:,:,0] * (conv[:,:,2] > 0.5)) * scale).astype('u1')).save(osp.join(path, '%03d.conv.png' % (i,)), 'PNG')
	recoo = rays.reconstruct_pcl(orig)
	recoi = rays.reconstruct_pcl(conv)
	np.savetxt(osp.join(path, '%03d.orig.txt' % (i,)), recoo.T)
	np.savetxt(osp.join(path, '%03d.conv.txt' % (i,)), recoi.T)


def process_one_part(path):
	input, output = load_data(path)
	dirpath = osp.splitext(path)[0]
	os.makedirs(dirpath, exist_ok=True)
	for i, (ii, oo) in enumerate(zip(input, output)):
		process_one_example(i, ii, oo, dirpath)


if __name__ == '__main__':
	path = osp.abspath(sys.argv[1])
	process_one_part(osp.join(path, 'gta2valeo.npz'))
	process_one_part(osp.join(path, 'valeo2gta.npz'))
