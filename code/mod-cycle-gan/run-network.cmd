::python train.py --Xtfr=../../datasets/anime/hidamari-sketch.tfrecord --Ytfr=../../datasets/anime/shinkai.tfrecord --Xname=hidamari-sketch --Yname=shinkai
::following line is for continuing learning a model
::python train.py --Xtfr=../../datasets/anime/hidamari-sketch.tfrecord --Ytfr=../../datasets/anime/shinkai.tfrecord --Xname=hidamari-sketch --Yname=shinkai --rundir=20180903-0020-0
::python train.py --Xtfr=../../datasets/real/ade20k.tfrecord,../../datasets/real/cosplay-music-videos.tfrecord,../../datasets/real/martin-fotky.tfrecord --Ytfr=../../datasets/anime/shinkai.tfrecord --Xname=real --Yname=anime > train.log
::python train.py --rundir=2019-01-05--00-48-0 --flagfile=E:\Projects\anime-style-transfer\checkpoint\2019-01-05--00-48-0\params.flagfile > train.log
::python train.py --Xtfr=../../datasets/real/ade20k.tfrecord --Ytfr=../../datasets/anime/no-game-no-life.tfrecord --Xname=real --Yname=anime > train.log
::python train.py --rundir=2019-01-08--01-38-0 --flagfile=E:\Projects\anime-style-transfer\checkpoint\2019-01-08--01-38-0\params.flagfile > train.log
::python train.py --Xtfr=../../datasets/real/ade20k.tfrecord --Ytfr=../../datasets/anime/no-game-no-life.tfrecord --Xname=real --Yname=anime --gantype=WGAN > train.log
::python train.py --rundir=2019-01-12--16-17-0 --flagfile=E:\Projects\anime-style-transfer\checkpoint\2019-01-12--16-17-0\params.flagfile > train.log
::python train.py --Xtfr=../../datasets/real/ade20k.tfrecord --Ytfr=../../datasets/anime/no-game-no-life.tfrecord --Xname=real --Yname=anime --gantype=GAN > train.log
::python train.py --rundir=2019-01-08--01-38-0 --flagfile=E:\Projects\anime-style-transfer\checkpoint\2019-01-08--01-38-0\params.flagfile > train.log
::python train.py --Xtfr=../../datasets/real/ade20k.tfrecord --Ytfr=../../datasets/anime/no-game-no-life.tfrecord --Xname=real --Yname=anime^
:: --gantype=LSGAN --XYgwl=1 --YXgwl=1 --Xdwl=1 --Ydwl=1 > train.log
::python train.py --rundir=2019-01-12--16-17-0 --flagfile=E:\Projects\anime-style-transfer\checkpoint\2019-01-12--16-17-0\params.flagfile > train.log
::python train.py --Xtfr=../../datasets/real/ade20k.tfrecord --Ytfr=../../datasets/anime/no-game-no-life.tfrecord --Xname=real --Yname=anime^
:: --gantype=WGAN --XYgwl=1 --YXgwl=1 --Xdwl=1 --Ydwl=1 > train.log
::python train.py --Xtfr=../../datasets/real/ade20k.tfrecord --Ytfr=../../datasets/anime/no-game-no-life.tfrecord --Xname=real --Yname=anime^
:: --gantype=GAN --XYgwl=1 --YXgwl=1 --Xdwl=1 --Ydwl=1 > train.log
python train.py --Xtfr=../../datasets/real/ade20k.tfrecord --Ytfr=../../datasets/anime/no-game-no-life.tfrecord --Xname=real --Yname=anime > train.log