set PYTHONPATH=%PYTHONPATH%;E:\Projects\anime-style-transfer\code\mod-cycle-gan

::python videos_to_images.py --videos_dir=../../../dataset-sources/anime/videos/no-game-no-life --images_dir=../../../dataset-sources/anime/images/no-game-no-life --period=1
::python videos_to_images.py --videos_dir=../../../dataset-sources/anime/videos/houseki-no-kuni --images_dir=../../../dataset-sources/anime/images/houseki-no-kuni --period=1
::python videos_to_images.py --videos_dir=../../../dataset-sources/anime/videos/hidamari-sketch --images_dir=../../../dataset-sources/anime/images/hidamari-sketch --period=1
::python videos_to_images.py --videos_dir=../../../dataset-sources/anime/videos/shinkai --images_dir=../../../dataset-sources/anime/images/shinkai --period=1
python videos_to_images.py --videos_dir=../../../dataset-sources/real/videos/cosplay-music-videos --images_dir=../../../dataset-sources/real/images/cosplay-music-videos --period=1
