import os
import sys
import numpy as np
import cv2

IMAGE_SIZE = 32

# 按照指定图像大小调整尺寸

# 读取训练数据
images = []
labels = []


def read_path(path_name):
    for dir_item in os.listdir(path_name):
        # 从初始路径开始叠加，合并成可识别的操作路径
        full_path = os.path.abspath(os.path.join(path_name, dir_item))

        if os.path.isdir(full_path):  # 如果是文件夹，继续递归调用
            read_path(full_path)
        else:  # 文件
            if dir_item.endswith('.jpg'):
                image = cv2.imread(full_path)
                image = cv2.resize(image, (IMAGE_SIZE, IMAGE_SIZE))

                # 放开这个代码，可以看到resize_image()函数的实际调用效果
                # cv2.imwrite('1.jpg', image)

                images.append(image)
                labels.append(path_name)

    return images, labels


# 从指定路径读取训练数据
def load_dataset(path_name):
    images, labels = read_path(path_name)

    # 将输入的所有图片转成四维数组，尺寸为(图片数量*IMAGE_SIZE*IMAGE_SIZE*3)
    # 图片为32 * 32像素,一个像素3个颜色值(RGB),总尺寸为90 * 32 * 32 *3
    images = np.array(images)
    print(images.shape)

    # 标注数据， ice -> 0 dirt->1 ordinary->2
    labels = np.array([
        0 if label.endswith('ice_road') else
        1 if label.endswith('dirt_road') else 2 for label in labels
    ])

    print(labels)

    return images, labels
