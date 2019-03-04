from math import log2

if __name__ == '__main__':
    base_size = (1080, 1920)
    ratio = base_size[1] / base_size[0]
    power = 4
    for i in range(round(1080 / 2**power)):
        print(2**power * i, 2**power * i * ratio)