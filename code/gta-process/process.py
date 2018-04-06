#!/usr/bin/env python3

import numpy as np
import json
from datetime import *
from PIL import Image
import os
import os.path as osp
import zipfile as zf
import io
import multiprocessing

DATA_DIR = osp.abspath('/datagrid/public_datasets/GTA_V/without_db_run_19/')
PCL_DIR = osp.abspath('/datagrid/personal/jasekota/gta-pcl')
BBOX = (140, 140, 140)
CAMERAS = np.array([[0, -0.4, 2, 1], [0 , -0.8, 0.6, 1], [0, -0.6, 2, 1], [0, -0.8, 0.6, 1]])
LIDARS = np.array(CAMERAS, copy=True)
LIDARS[:,1] += 1
THREADS = 16
DATA_SIZE = 4000

ALL_FILES = [osp.join(DATA_DIR, f) for f in os.listdir(DATA_DIR)]
JSONS = sorted([f for f in ALL_FILES if f.endswith('.json')])
DEPTHS = sorted([f for f in ALL_FILES if '-depth' in f])

assert len(DEPTHS) == len(JSONS)

if DATA_SIZE is not -1:
    DEPTHS = DEPTHS[:DATA_SIZE]
    JSONS = JSONS[:DATA_SIZE]

def depth_to_pcl(depth, proji, viewi, bbox):
    height, width = depth.shape
    y, x = np.where(depth > 0.0)
    y_data = (-2 / height) * y + 1
    x_data = (2 / width) * x - 1
    points = np.ones((4, len(y)))
    points[0,:] = y_data
    points[1,:] = x_data
    points[2,:] = depth[y, x]
    
    ego_points = proji @ points
    ego_points /= ego_points[3,:]
    
    ego_points = ego_points[:,(np.abs(ego_points[0,:]) < bbox[0]) & (np.abs(ego_points[1,:]) < bbox[1]) & (np.abs(ego_points[2,:]) < bbox[2])]
    
    world_points = viewi @ ego_points
    world_points /= world_points[3,:]
    
    return world_points[:3,:].astype(np.float32)

os.makedirs(PCL_DIR, exist_ok=True)

def process(i, json_file, depth_im):
    global CAMERAS, BBOX, PCL_DIR, LIDARS
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
    world = depth_to_pcl(depth_real, np.linalg.inv(proj_matrix), vinv, BBOX)
    car_center = ((vinv @ CAMERAS[i % 4,:]).T)[:3].tolist()
    lidar_center = ((vinv @ LIDARS[i % 4,:]).T)[:3].tolist()
    tmp_meta = {'timestamp': datetime.strptime(json_data['imagepath'], '%Y-%m-%d--%H-%M-%S--%f').timestamp(),
                    'pcl_id' : i,
                    'bbox' : BBOX,
                    'car_center' : car_center,
                    'lidar_center' : lidar_center,
                    'orig_json' : json_data}
    with zf.ZipFile(osp.join(PCL_DIR,'%05d.zip' % (i,)), 'w', zf.ZIP_LZMA) as cf:
        cf.writestr('metadata.json', json.dumps(tmp_meta))
        tmpf = io.BytesIO()
        np.save(tmpf, world)
        cf.writestr('pcl.npy', tmpf.getvalue())

with multiprocessing.Pool(THREADS) as pool:
    pool.starmap(process, zip(range(len(DEPTHS)), JSONS, DEPTHS))
