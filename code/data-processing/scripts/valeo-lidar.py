#!/usr/bin/env python3

import os
import os.path as osp
import numpy as np
import multiprocessing as mp
import sys
sys.path.append(osp.abspath('..'))
import rays
import datapool as dp

def process_file(indir, outfile):
    fname = osp.basename(indir)
    all_files = [osp.join(indir, f) for f in os.listdir(indir) if f.endswith('.zip')]
    dapool = dp.DataPool(all_files, -1)
    data = []
    for i, ts in enumerate(dapool.tss):
        data.append(rays.get_lidar_data(dapool, ts, np.array([0, 0, 2.]), 0.005).astype('<f4'))
        print('Got array %d for file %s' % (i, fname))
        sys.stdout.flush()
    data = np.array(data)
    np.save(outfile, data)


if __name__ == '__main__':
    indir = osp.abspath(sys.argv[1])
    outdir = osp.abspath(sys.argv[2])
    if len(sys.argv) > 3:
        num_threads = int(sys.argv[3])
    else:
        num_threads = int(0.5 * mp.cpu_count())
    infiles = [osp.join(indir, f) for f in os.listdir(indir) if 'conv' in f]
    outfiles = [osp.join(outdir, osp.basename(f)) + '.npy' for f in infiles]
    os.makedirs(outdir, mode=0o755, exist_ok=True)

    with mp.Pool(num_threads) as pool:
        pool.starmap(process_file, zip(infiles, outfiles))
