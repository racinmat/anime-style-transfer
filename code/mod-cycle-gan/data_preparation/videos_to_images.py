import cv2
import progressbar
import tensorflow as tf
import glob
import os
import os.path as osp


def get_base_name(name):
    return osp.basename(os.path.splitext(name)[0])


# todo: prefixovat názvy obrázků názvem videa
def extract_video(video_path, images_dir):
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
    second = int(frame / framerate)
    while success:
        success, image = vidcap.read()
        filename = osp.join(images_dir, "{}-frame-{}.png".format(video_name, frame))
        frame += 1
        prev_second = second
        second = int(frame / framerate)
        # every second take screenshot
        if second == prev_second:
            continue
        if osp.exists(filename):
            continue
        pbar.update(frame - 1)
        cv2.imwrite(filename, image)


if __name__ == '__main__':
    videos_root = '../../dataset-sources/anime/videos'
    images_root = '../../dataset-sources/anime/images'
    for dir_name in os.listdir(videos_root):
        print('processing directory {}'.format(dir_name))
        for filename in glob.glob(osp.join(videos_root, dir_name, '*.mkv')):
            print('processing file {}'.format(filename))
            extract_video(filename, os.path.join(images_root, dir_name))
