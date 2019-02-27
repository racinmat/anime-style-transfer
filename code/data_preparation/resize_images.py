from PIL import Image
from scipy.misc import imresize, imsave
import numpy as np

if __name__ == '__main__':
    images_map = {
        '31515792_1916092565090807_4980512332061868032_o.jpg': '0-out.png',
        '19983424_1613083228725077_195972743492877576_o.jpg': '1-out.png',
        '19956635_1613085545391512_5518005744105127840_o.jpg': '2-out.png',
        '17504383_1481397145227020_8972123734668384543_o.jpg': '3-out.png',
        '31398165_1916103011756429_7718895562010918912_o.jpg': '4-out.png',
        '18836720_1561674283865972_7458354995502481745_o.jpg': '5-out.png',
        '31398292_1916110188422378_4008514452193280000_o.jpg': '6-out.png',
        '31501002_1916120825087981_6537731864294064128_o.jpg': '7-out.png',
        '19956829_1613081782058555_3672919675013192_o.jpg': '8-out.png',
        '31506214_1916105401756190_795451024373448704_o.jpg': '9-out.png',
        '21743510_1677421842291215_2356284401053964401_o.jpg': '10-out.png',
        '21014172_1655932461106820_8611403578015781627_o.jpg': '11-out.png',
        '31396193_1916092058424191_1971482631308574720_o.jpg': '12-out.png',
        '31400921_1916099725090091_8712899024648142848_o.jpg': '13-out.png',
        '12314542_1089564397743632_2757526142271768138_o.jpg': '14-out.png',
        '31392326_1916095491757181_1352218926422949888_o.jpg': '15-out.png',
        '17546976_1481822715184463_3246916780779375477_o.jpg': '16-out.png',
    }

    for name_orig, name_square in images_map.items():
        im_orig = np.array(Image.open(name_orig))
        im_square = np.array(Image.open(name_square))
        # do only shrink, not expand to prevent
        orig_shape = im_orig.shape[0:2]
        ratio = max(im_square.shape[0:2]) / max(orig_shape)
        new_im = imresize(im_square, (int(orig_shape[0] * ratio), int(orig_shape[1] * ratio)))
        imsave('fixed-'+name_square, new_im)
