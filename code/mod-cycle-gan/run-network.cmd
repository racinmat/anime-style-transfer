::python train.py --Xtfr=../../datasets/anime/hidamari-sketch.tfrecord --Ytfr=../../datasets/anime/shinkai.tfrecord --Xname=hidamari-sketch --Yname=shinkai
::following line is for continuing learning a model
python train.py --Xtfr=../../datasets/anime/hidamari-sketch.tfrecord --Ytfr=../../datasets/anime/shinkai.tfrecord --Xname=hidamari-sketch --Yname=shinkai --rundir=20180903-0020-0