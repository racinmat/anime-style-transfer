"""
Script for transforming images to videos.

Examples:
python images_to_videos.py --images_dir=../../../data/images/animefest-2016/20180625-1659-0/80000 --videos_dir=../../../data/videos
"""
from itertools import groupby

import cv2
import progressbar
import tensorflow as tf
import glob
import os
import os.path as osp


FLAGS = tf.flags.FLAGS

tf.flags.DEFINE_string('videos_dir', '../../dataset-sources/anime/videos', 'Path to new video.')
tf.flags.DEFINE_string('images_dir', '../../dataset-sources/anime/images', 'Directory where to output images.')
tf.flags.DEFINE_float('fps', 30.0, 'FPS of resulting video.')


def get_base_name(name):
    return osp.basename(os.path.splitext(name)[0])


def find_files_and_create_videos(images_dir, videos_dir):
    images = list(sorted([img for img in os.listdir(images_dir) if img.endswith(".png") or img.endswith(".jpg")]))
    # images are sorted lexicographically, because I number them with padding
    videos = groupby(images, key=lambda x: x.split('-frame-')[0])
    for video_name, images in videos:
        os.makedirs(videos_dir, exist_ok=True)
        create_video(images_dir, list(images), osp.join(videos_dir, video_name+'.avi'))


def create_video(images_dir, images, video_path):
    print(images[0])
    frame = cv2.imread(osp.join(images_dir, images[0]))
    height, width, layers = frame.shape

    # video = cv2.VideoWriter(video_path, -1, 1, (width, height))
    fourcc = cv2.VideoWriter_fourcc(*'DIVX')
    fps = FLAGS.fps
    video = cv2.VideoWriter(video_path, fourcc, fps, (width, height))

    widgets = [progressbar.Percentage(), ' ', progressbar.Counter(), ' ', progressbar.Bar(), ' ',
               progressbar.FileTransferSpeed()]
    pbar = progressbar.ProgressBar(widgets=widgets, max_value=len(images)).start()

    for i, image in enumerate(images):
        pbar.update(i)
        video.write(cv2.imread(osp.join(images_dir, image)))
    pbar.finish()

    cv2.destroyAllWindows()
    video.release()


def main(_):
    print('processing directory {}'.format(FLAGS.images_dir))
    find_files_and_create_videos(FLAGS.images_dir, FLAGS.videos_dir)


if __name__ == '__main__':
    tf.app.run()
