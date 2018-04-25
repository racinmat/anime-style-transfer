#!/usr/bin/env python3

import os
import os.path as osp
import rays
import numpy as np
import datapool as dp
import multiprocessing as mp
import itertools as it
import sys

DIR = osp.abspath('/datagrid/personal/jasekota/dip-dataset/gta/zipfiles')
OUT_NPY = osp.abspath('/datagrid/personal/jasekota/dip-dataset/gta/npys')
FILES = [osp.join(DIR, f) for f in os.listdir(DIR) if f.endswith('.zip')]
COMPLETED = sorted([int(f[:5]) for f in os.listdir(OUT_NPY) if f.endswith('.npy')])
THREADS = 20

def _func_wrap(timestamp, i):
    global dapool, OUT_NPY
    data = rays.get_lidar_data(dapool, timestamp, np.array([0, 0, .75], 0.05))
    np.save(osp.join(OUT_NPY, '%05d.npy' % (i,)), data)
    print('Got one array at ts %.5f with id %05d!' % (timestamp, i))
    sys.stdout.flush()

dapool = dp.GTADataPool(FILES)
ts = np.arange(dapool.tss[0], dapool.tss[-1], 0.1)
all_ids = np.arange(ts.size)
unfinished = np.setdiff1d(all_ids, COMPLETED)
mid_point = int(unfinished.size/2)

with mp.Pool(THREADS) as pool:
    pool.starmap(_func_wrap, zip(ts[unfinished[:mid_point]], unfinished[:mid_point]))
