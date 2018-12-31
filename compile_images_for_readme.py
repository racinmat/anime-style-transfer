import matplotlib.pyplot as plt
from PIL import Image

if __name__ == '__main__':
    # todo: compile results for readme to table as in https://github.com/junyanz/CycleGAN
    image_pairs = {
        'data/images/2018-06-25--16-59-0/200000/03-in.png': 'data/images/2018-06-25--16-59-0/200000/03-out.png',
        'data/images/2018-06-25--16-59-0/200000/05-in.png': 'data/images/2018-06-25--16-59-0/200000/05-out.png',
        'data/images/2018-06-25--16-59-0/200000/06-in.png': 'data/images/2018-06-25--16-59-0/200000/06-out.png',
        'data/images/2018-06-25--16-59-0/200000/11-in.png': 'data/images/2018-06-25--16-59-0/200000/11-out.png',
        'data/images/2018-06-25--16-59-0/200000/15-in.png': 'data/images/2018-06-25--16-59-0/200000/15-out.png',
    }

    fig = plt.figure(figsize=(6, 12))
    for i, (path_from, path_to) in enumerate(image_pairs.items()):
        img_from = Image.open(path_from)
        img_to = Image.open(path_to)
        ax = fig.add_subplot(len(image_pairs), 2, 2*i+1)
        ax.imshow(img_from)
        ax.axis('off')
        if i == 0:
            ax.set_title('real', {'fontsize': 20})

        ax = fig.add_subplot(len(image_pairs), 2, 2*i+2)
        ax.imshow(img_to)
        ax.axis('off')
        if i == 0:
            ax.set_title('anime', {'fontsize': 20})

    plt.tight_layout()
    plt.savefig('images.png')
    pass
