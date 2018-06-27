"""
Script for transforming videos to images.
Either takes all frames, or samples frame every X seconds.

Params:
    --period=5  takes frame every 5 seconds
    --videos_dir=<path>  directory with videos
    --images_dir=<path>  directory with videos

Examples:
    python videos_to_images.py --period=1 --videos_dir=../../dataset-sources/anime/videos
    python3 videos_to_images.py --videos_dir=../../../dataset-sources/real/videos/animefest-2016 --images_dir=../../../dataset-sources/real/images/animefest-2016
"""

import cv2
import progressbar
import tensorflow as tf
import glob
import os
import os.path as osp


FLAGS = tf.flags.FLAGS

tf.flags.DEFINE_integer('period', None, 'If not none, next sample is taken from video after <period> seconds.')
tf.flags.DEFINE_string('videos_dir', '../../dataset-sources/anime/videos', 'Directory with videos.')
tf.flags.DEFINE_string('images_dir', '../../dataset-sources/anime/images', 'Directory where to output images.')


def get_base_name(name):
    return osp.basename(os.path.splitext(name)[0])


def extract_video(video_path, images_dir):
    if not osp.exists(images_dir):
        os.makedirs(images_dir)

    sampling_mode = FLAGS.period is not None

    period = FLAGS.period
    video_name = get_base_name(video_path)
    vidcap = cv2.VideoCapture(video_path)
    framerate = vidcap.get(cv2.CAP_PROP_FPS)
    print("framerate:", framerate)
    framecount = vidcap.get(cv2.CAP_PROP_FRAME_COUNT)
    print("framecount:", framecount)
    width = vidcap.get(cv2.CAP_PROP_FRAME_WIDTH)
    print("width:", width)
    height = vidcap.get(cv2.CAP_PROP_FRAME_HEIGHT)
    print("height:", height)
    length = framecount / framerate
    print("length in sec:", length)
    print("length in min:", length / 60)

    widgets = [progressbar.Percentage(), ' ', progressbar.Counter(), ' ', progressbar.Bar(), ' ',
               progressbar.FileTransferSpeed()]
    pbar = progressbar.ProgressBar(widgets=widgets, max_value=int(framecount)).start()

    success = True
    frame = 0
    prev_second = 0
    while success:
        success, image = vidcap.read()
        filename = osp.join(images_dir, "{}-frame-{}.png".format(video_name, str(frame).zfill(6)))
        frame += 1
        second = int(frame / framerate)
        if sampling_mode:
            # every second take screenshot
            if (second - prev_second) != period:
                continue
        if osp.exists(filename):
            continue

        pbar.update(frame - 1)
        cv2.imwrite(filename, image)
        prev_second = second


def main(_):
    # videos_root = FLAGS.videos_dir
    # images_root = FLAGS.images_dir
    # for dir_name in os.listdir(videos_root):
    #     print('processing directory {}'.format(dir_name))
    #     filenames = list(glob.glob(osp.join(videos_root, dir_name, '*.mkv')))
    #     filenames += list(glob.glob(osp.join(videos_root, dir_name, '*.mp4')))
    #     for filename in filenames:
    #         print('processing file {}'.format(filename))
    #         extract_video(filename, os.path.join(images_root, dir_name))
    print('processing directory {}'.format(FLAGS.videos_dir))
    filenames = list(glob.glob(osp.join(FLAGS.videos_dir, '**', '*.mkv'), recursive=True))
    filenames += list(glob.glob(osp.join(FLAGS.videos_dir, '**', '*.mp4'), recursive=True))
    filenames += list(glob.glob(osp.join(FLAGS.videos_dir, '**', '*.flv'), recursive=True))
    for filename in filenames:
        print('processing file {}'.format(filename))
        extract_video(filename, FLAGS.images_dir)


if __name__ == '__main__':
    tf.app.run()
