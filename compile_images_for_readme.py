import matplotlib.pyplot as plt
from PIL import Image

if __name__ == '__main__':
    image_pairs = {
        # 'data/images/2018-06-25--16-59-0/200000/03-in.png': 'data/images/2018-06-25--16-59-0/200000/03-out.png',
        # 'data/images/2018-06-25--16-59-0/200000/05-in.png': 'data/images/2018-06-25--16-59-0/200000/05-out.png',
        # 'data/images/2018-06-25--16-59-0/200000/06-in.png': 'data/images/2018-06-25--16-59-0/200000/06-out.png',
        # 'data/images/2018-06-25--16-59-0/200000/11-in.png': 'data/images/2018-06-25--16-59-0/200000/11-out.png',
        # 'data/images/2018-06-25--16-59-0/200000/15-in.png': 'data/images/2018-06-25--16-59-0/200000/15-out.png',
        'data/images/lena_in/30420006_2053591314669670_114488091342823155_o.jpg':
            'data/images/lena_out/2018-06-25--16-59-0/200000/16-out.png',
        'data/images/lena_in/33436603_2094902623871872_7274095563287560192_o.jpg':
            'data/images/lena_out/2018-06-25--16-59-0/200000/17-out.png',
        'data/images/lena_in/12314200_1180852525276891_8490887074540900127_o.jpg':
            'data/images/lena_out/2018-06-25--16-59-0/200000/00-out.png',
        'data/images/lena_in/34457569_2107316209297180_364960813003309056_o.jpg':
            'data/images/lena_out/2018-06-25--16-59-0/200000/19-out.png',
    }

    fig = plt.figure(figsize=(15, 30))
    for i, (path_from, path_to) in enumerate(image_pairs.items()):
        for j, (path, label) in enumerate(zip([path_from, path_to], ['real', 'anime'])):
            img = Image.open(path)
            ax = fig.add_subplot(len(image_pairs), 2, 2 * i + j + 1)
            ax.imshow(img)
            ax.axis('off')
            ax.set_xticklabels([])
            ax.set_yticklabels([])
            ax.set_aspect('equal')
            if i == 0:
                ax.set_title(label, {'fontsize': 20})

    plt.tight_layout()
    plt.savefig('images-testset.png')
