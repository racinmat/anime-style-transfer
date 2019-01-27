set PYTHONPATH=%PYTHONPATH%;E:\Projects\anime-style-transfer\code\mod-cycle-gan

python images_to_tfrecord.py --type=real --name=ade20k
python images_to_tfrecord.py --type=anime --name=houseki-no-kuni
python images_to_tfrecord.py --type=anime --name=no-game-no-life
python images_to_tfrecord.py --type=anime --name=hidamari-sketch
python images_to_tfrecord.py --type=anime --name=shinkai
python images_to_tfrecord.py --type=real --name=martin-fotky
python images_to_tfrecord.py --type=real --name=mashiiro-fotky
python images_to_tfrecord.py --type=real --name=cosplay-music-videos
python images_to_tfrecord.py --type=real --name=diff_sizes
