#!/usr/bin/env python3

import numpy as np
from PIL import Image
import os
import os.path as osp
import sys
sys.path.append(osp.join('..'))
import rays

scale_depth = 255.0/131.0
scale_inten = 255.0

def load_data(filename, limit=100):
    arr = np.load(filename)
    indata = arr['input'][:limit].copy()
    output = arr['output'][:limit].copy()
    del arr
    return indata, output


def process_one_example(i, orig, conv, path):
    Image.fromarray(((orig[:,:,0] * (orig[:,:,2] > 0.5)) * scale_depth).astype('u1')).save(osp.join(path, '%03d.depth.orig.png' % (i,)), 'PNG')
    Image.fromarray(((conv[:,:,0] * (conv[:,:,2] > 0.5)) * scale_depth).astype('u1')).save(osp.join(path, '%03d.depth.conv.png' % (i,)), 'PNG')
    Image.fromarray(((orig[:,:,1] * (orig[:,:,2] > 0.5)) * scale_inten).astype('u1')).save(osp.join(path, '%03d.inten.orig.png' % (i,)), 'PNG')
    Image.fromarray(((conv[:,:,1] * (conv[:,:,2] > 0.5)) * scale_inten).astype('u1')).save(osp.join(path, '%03d.inten.conv.png' % (i,)), 'PNG')
    recoo = rays.reconstruct_pcl(orig)
    recoi = rays.reconstruct_pcl(conv)
    np.savetxt(osp.join(path, '%03d.pcl.orig.txt' % (i,)), recoo.T)
    np.savetxt(osp.join(path, '%03d.pcl.conv.txt' % (i,)), recoi.T)


def process_one_part(path, items):
    indata, output = load_data(path, items)
    dirpath = osp.splitext(path)[0]
    os.makedirs(dirpath, exist_ok=True)
    for i, (ii, oo) in enumerate(zip(indata, output)):
        process_one_example(i, ii, oo, dirpath)


if __name__ == '__main__':
    path = osp.abspath(sys.argv[1])
    items = 100
    if len(sys.argv) > 2:
        items = int(sys.argv[2])
    process_one_part(osp.join(path, 'gta2valeo.npz'), items)
    process_one_part(osp.join(path, 'valeo2gta.npz'), items)
