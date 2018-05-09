#!/usr/bin/env python

import os
import os.path as osp
import multiprocessing as mp
import itertools as it
import zip_processing as zp


DATADIR = osp.abspath('/datagrid/public_datasets/GTA_V/generic-dataset-ota')
OUTDIR = osp.abspath('/datagrid/personal/jasekota/dip-dataset/gta/zipfiles')
THREADS = 28
ALL_FILES = [osp.join(DATADIR, f) for f in os.listdir(DATADIR)]

JSONS = sorted([f for f in ALL_FILES if f.endswith('.json')])
DEPTHS = sorted([f for f in ALL_FILES if 'depth' in f])
RGBS = sorted([f for f in ALL_FILES if 'png' in f and 'depth' not in f and 'stencil' not in f])

assert len(JSONS) == len(DEPTHS)

iter_data = zip(it.count(), JSONS, DEPTHS, RGBS, it.repeat(zp.BBOX), it.repeat(OUTDIR))

with mp.Pool(THREADS) as pool:
    pool.starmap(zp.make_gta_zip, iter_data)
