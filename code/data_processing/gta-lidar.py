#!/usr/bin/env python3

import os
import os.path as osp
import rays
import datapool as dp
import multiprocessing as mp
import itertools as it

DIR = osp.abspath('/datagrid/personal/jasekota/dip-dataset/gta/zipfiles')
OUT_NPY = osp.abspath('/datagrid/personal/jasekota/dip-dataset/gta/npys')
FILES = [osp.join(DIR, f) for f in os.listdir(DIR) if f.endswith('.zip')]
THREADS = 16

def _func_wrap(timestamp, i):
    global dapool, OUT_NPY
    data = rays.get_lidar_data(dapool, timestamp).astype('<f4')
    np.save(osp.join(OUT_NPY, '%05d.npy' % (i,)), data)

dapool = dp.GTADataPool(FILES)
ts = np.arange(dapool.tss[0], dapool.tss[-1], 0.2)

with mp.Pool(THREADS) as pool:
    pool.map(_func_wrap, zip(ts, it.count())
