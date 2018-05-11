#!/usr/bin/env python3

import os
import os.path as osp
import multiprocessing as mp
import itertools as it
import sys
sys.path.append(osp.abspath('..'))
import zip_processing as zp

if __name__ == '__main__':
    indir = osp.abspath(sys.argv[1])
    outdir = osp.abspath(sys.argv[2])
    if len(sys.argv) > 3:
        num_threads = int(sys.argv[3])
    else:
        num_threads = int(0.5 * mp.cpu_count())
    allfiles = [osp.join(indir, f) for f in os.listdir(indir)]

    jsons = sorted([f for f in allfiles if f.endswith('.json')])
    depths = sorted([f for f in allfiles if 'depth' in f])
    rgbs = sorted([f for f in allfiles if 'png' in f and 'depth' not in f and 'stencil' not in f])

    assert len(jsons) == len(depths) == len(rgbs)

    iter_data = zip(it.count(), jsons, depths, rgbs, it.repeat(zp.BBOX), it.repeat(outdir))

    with mp.Pool(num_threads) as pool:
        pool.starmap(zp.make_gta_zip, iter_data)
