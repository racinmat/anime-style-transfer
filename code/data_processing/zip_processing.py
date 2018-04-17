#!/usr/bin/env python3

import json
from datetime import datetime
import os
import os.path as osp
import zipfile as zf
import io
import subprocess
import numpy as np
from PIL import Image

BBOX = (140, 140, 140)

def depth_to_pcl(depth, proji, viewi, bbox):
    height, width = depth.shape
    y, x = np.where(depth > 0.0)
    y_data = (-2 / height) * y + 1
    x_data = (2 / width) * x - 1
    points = np.ones((4, len(y)))
    points[0, :] = y_data
    points[1, :] = x_data
    points[2, :] = depth[y, x]

    ego_points = proji @ points
    ego_points /= ego_points[3, :]

    ego_points = ego_points[:, (np.abs(ego_points[0, :]) < bbox[0]) &
                               (np.abs(ego_points[1, :]) < bbox[1]) &
                               (np.abs(ego_points[2, :]) < bbox[2])]

    world_points = viewi @ ego_points
    world_points /= world_points[3, :]

    return world_points[:3, :].astype(np.float32)


def make_gta_zip(i, json_file, depth_im, outdir, bbox):
    with open(json_file, mode='r') as f:
        json_data = json.load(f)
    assert json_data['imagepath'] + '-depth.png' == osp.basename(depth_im)
    del json_data['entities']
    del json_data['height']
    del json_data['width']
    del json_data['timestamp']
    del json_data['camera_fov']
    del json_data['snapshot_id']
    depth_int = np.array(Image.open(depth_im), dtype=np.uint16)
    depth_real = np.array(depth_int/np.iinfo(np.uint16).max, dtype=np.float32)
    proj_matrix = np.array(json_data['proj_matrix'])
    view_matrix = np.array(json_data['view_matrix'])
    vinv = np.linalg.inv(view_matrix)
    world = depth_to_pcl(depth_real, np.linalg.inv(proj_matrix), vinv, bbox)
    lidar_center = np.array(json_data['camera_pos']).tolist()
    tmp_meta = {'timestamp': datetime.strptime(json_data['imagepath'],
                                               '%Y-%m-%d--%H-%M-%S--%f').timestamp(),
                    'pcl_id' : i,
                    'bbox' : bbox,
                    'lidar_center' : lidar_center,
                    'orig_json' : json_data}
    with zf.ZipFile(osp.join(outdir, '%04d.zip' % (i,)), 'w', zf.ZIP_LZMA) as cf:
        cf.writestr('metadata.json', json.dumps(tmp_meta))
        tmpf = io.BytesIO()
        np.save(tmpf, world)
        cf.writestr('pcl.npy', tmpf.getvalue())
        

def make_valeo_zips(datfile, binary, outputdir, remove=True):
    datfile = osp.abspath(datfile)
    outputdir = osp.abspath(outputdir)
    fname = osp.basename(datfile)
    outdir = osp.join(outputdir, osp.splitext(fname)[0])
    if not osp.exists(outdir):
        subprocess.run([binary, '-i', datfile, '-o', outdir])
    files = os.listdir(outdir)
    matrices = sorted([f for f in files if f.endswith('.matrix')])
    tss = sorted([f for f in files if f.endswith('.ts')])
    ptss = sorted([f for f in files if f.endswith('.pts')])

    for i, (matrix, tsname, pts) in enumerate(zip(matrices, tss, ptss)):
        pts_np = np.fromfile(osp.join(outdir, pts), dtype='<f4').reshape((-1, 4)).T
        matr_np = np.fromfile(osp.join(outdir, matrix), dtype='<f8').reshape((4, 4))
        matr_np[:3, 3] /= 1000
        ts = np.fromfile(osp.join(outdir, tsname), dtype='<i8').tolist()[0]
        saved_intensity = pts_np[3, :].copy()
        pts_np[3, :] = 1.0
        pts_np = (matr_np @ pts_np).astype('<f4')
        pts_np[3, :] = saved_intensity.copy()
        meta = {'timestamp': ts / 1000000.0,
                'pcl_id' : i,
                'lidar_center': matr_np[:3, 3].tolist(),
                'matrix': matr_np.tolist()}
        with zf.ZipFile(osp.join(outdir, '%04d.zip' % (i,)), 'w', zf.ZIP_LZMA) as cf:
            cf.writestr('metadata.json', json.dumps(meta))
            tmpf = io.BytesIO()
            np.save(tmpf, pts_np)
            cf.writestr('pcl.npy', tmpf.getvalue())
        if remove:
            os.remove(osp.join(outdir, pts))
            os.remove(osp.join(outdir, matrix))
            os.remove(osp.join(outdir, tsname))
