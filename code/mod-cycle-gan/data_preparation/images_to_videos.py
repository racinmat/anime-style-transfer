"""
Script for transforming images to videos.

Examples:
python images_to_videos.py --images_dir=../../../data/images/animefest-2016/20180625-1659-0/80000 --video_path=../../../data/videos/animefest.mp4
python images_to_videos.py --images_dir=../../../data/images/animefest-2016/20180625-1659-0/80000 --video_path=../../../data/videos/animefest.avi
"""

import cv2
import progressbar
import tensorflow as tf
import glob
import os
import os.path as osp


FLAGS = tf.flags.FLAGS

tf.flags.DEFINE_string('video_path', '../../dataset-sources/anime/videos', 'Path to new video.')
tf.flags.DEFINE_string('images_dir', '../../dataset-sources/anime/images', 'Directory where to output images.')
tf.flags.DEFINE_float('fps', 30.0, 'FPS of resulting video.')


def get_base_name(name):
    return osp.basename(os.path.splitext(name)[0])


def create_video(images_dir, video_path):
    images = sorted([img for img in os.listdir(images_dir) if img.endswith(".png") or img.endswith(".jpg")])
    images.sort(key=lambda x: os.path.getmtime(osp.join(images_dir, x)))    # when files are not sorted lexicographically, but by creation date
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
    create_video(FLAGS.images_dir, FLAGS.video_path)


if __name__ == '__main__':
    tf.app.run()
