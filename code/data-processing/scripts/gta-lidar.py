#!/usr/bin/env python3

import os
import os.path as osp
import sys
sys.path.append(osp.abspath('..'))
import numpy as np
import multiprocessing as mp
import itertools as it
import rays
import datapool as dp


def process_file(timestamp, i, outdir):
    global dapool
    data = rays.get_lidar_data(dapool, timestamp, np.array([0, 0, 1.0]), 0.005)
    data = rays.interpolate_lidar(data, 0.4, 3)
    np.save(osp.join(outdir, '%05d.npy' % (i,)), data)
    print('Got one array at ts %.5f with id %05d!' % (timestamp, i))
    sys.stdout.flush()

if __name__ == '__main__':
    indir = osp.abspath(sys.argv[1])
    outdir = osp.abspath(sys.argv[2])
    if len(sys.argv) > 3:
        num_threads = int(sys.argv[3])
    else:
        num_threads = int(0.5 * mp.cpu_count())
    allfiles = [osp.join(indir, f) for f in os.listdir(indir) if f.endswith('.zip')]
    dapool = dp.GTADataPool(allfiles)
    ts = np.arange(dapool.tss[0], dapool.tss[-1], 0.1)
    iter_data = zip(ts, it.count(), it.repeat(outdir))

    with mp.Pool(num_threads) as pool:
        pool.starmap(process_file, iter_data)
