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
THREADS = 32

def _func_wrap(timestamp, i):
    global dapool, OUT_NPY
    data = rays.get_lidar_data(dapool, timestamp, np.array([0, 0, 1.0]), 0.005)
    data = rays.interpolate_lidar(data, 0.4, 3)
    np.save(osp.join(OUT_NPY, '%05d.npy' % (i,)), data)
    print('Got one array at ts %.5f with id %05d!' % (timestamp, i))
    sys.stdout.flush()

dapool = dp.GTADataPool(FILES)
ts = np.arange(dapool.tss[0], dapool.tss[-1], 0.1)

iter = zip(ts, it.count())


with mp.Pool(THREADS) as pool:
    pool.starmap(_func_wrap, iter)
