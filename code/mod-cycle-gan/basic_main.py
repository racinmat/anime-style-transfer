#!/usr/bin/env python3

import logging
logging.basicConfig(level=logging.INFO)
import cycle
import cycle.models.lidar as lidar
valeotfr = cycle.utils.TFReader('/datagrid/personal/jasekota/dip-dataset/valeo/valeo-trn.tfrecords', 'valeo', lidar.DATA_SHAPE, normer=lidar.utils.normer, denormer=lidar.utils.denormer)
gtatfr = cycle.utils.TFReader('/datagrid/personal/jasekota/dip-dataset/gta/gta-trn.tfrecords', 'gta', lidar.DATA_SHAPE, normer=lidar.utils.normer, denormer=lidar.utils.denormer)
valeogan = cycle.nets.LSGAN(lidar.Generator('valeo-gen', True, 0), lidar.Discriminator('valeo-dis', True, 0), lidar.DATA_SHAPE, lidar.DATA_SHAPE, 2, 2)
gtagan = cycle.nets.LSGAN(lidar.Generator('gta-gen', True, 0), lidar.Discriminator('gta-dis', True, 0), lidar.DATA_SHAPE, lidar.DATA_SHAPE, 2, 2)
cygan = cycle.CycleGAN(valeogan, gtagan, 'valeo', 'gta', valeotfr, gtatfr, cycle_lambda=5.0, visualizer=lidar.visualise)
cygan.train('/datagrid/personal/jasekota/cycle-dip/cp/', 5, 1)
