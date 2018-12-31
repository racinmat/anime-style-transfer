Real world to anime style transfer
==================================

This is repository of TensorFlow implementation of CycleGAN:
https://arxiv.org/abs/1703.10593 CycleGAN is GAN-like neural network for style
transfer, which does not require paired training data.

This implementation is heavily based on Otakar Jašek's diploma thesis
https://dspace.cvut.cz/handle/10467/77173

It uses 2 datasets: real and anime. Real data are from common machine learning
datasets for computer vision. Namely ade20k. Anime data are from anime videos,
sampled at 1 FPS.

Code is in `code/mod-cycle-gan`. `code/mod-cycle-gan/data_preparation` contains
data preparation scripts. The input to neural network is native TensorFlow
format, protobuf. Videos are sampled to obtain images and then images are
converted into `.tfrecord` files containing protobuf format of training data.

Neural network is then trained in `code/mod-cycle-gan/train.py` by feeding it
two tfrecord files, one with real data, one with anime data. Trained network
then can be used for inference, transforming real images to anime data by
`code/mod-cycle-gan/transform.py` script.

Example for start training:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
python3 train.py --batchsize=2 --Ytfr=../../datasets/anime/houseki-no-kuni.tfrecord
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

if you need to run training on server, in background, you can use
`run-network-bg.sh` script. For example, you can run the same network in
background by:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
./run-network-bg.sh --batchsize=2 --Ytfr='../../datasets/anime/houseki-no-kuni.tfrecord'
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Trained network is stored in `.pb` files, which contain its very compact,
protobuf representation. It is much smaller than checkpoints, so it can be even
versioned in git.

Trained networks are stored in `export/<network name>/<current training step>`

When transforming video, we must split it to images, transform them, and then
create video from them. Using this approach, audio is lost, obviously. Commands
for that, example:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
python data_preparation/videos_to_images.py --videos_dir=../../dataset-sources/real/videos/animefest-2017-cosplay --images_dir=../../dataset-sources/real/images/animefest-2017-cosplay
python transform.py --inpath=../../dataset-sources/real/images/animefest-2017-cosplay/*.png --outdir=../../data/images/animefest-2017-cosplay --includein=0 --rundir=20180625-1659-0
python data_preparation/images_to_videos.py --images_dir=../../data/images/animefest-2017-cosplay/20180625-1659-0/80000 --video_path=../../data/videos/animefest-cosplay.avi
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Images extracted from videos take lots of space, and are not needed when
threcords are generated so you can delete them.

## Results (so far)

Trained on 2 datasets: [Ade20k](http://groups.csail.mit.edu/vision/datasets/ADE20K/) and anime series + movie [No Game No Life](https://myanimelist.net/anime/19815/No_Game_No_Life), 
I obtained following results on Ade20k dataset (training data):
![Image of results](https://raw.github.com/racinmat/anime-style-transfer/master/ade20k-images.png)
