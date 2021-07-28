#_*_coding:utf-8_*_
import os.path,glob,os
import numpy as np
from PIL import Image

def make_rp(i):
    im = np.array(Image.open(i).convert("L"))
    a = np.asarray(im).astype('float')
    vec_el = np.pi / 2.2  # 光源的俯视角度，弧度值
    vec_az = np.pi / 4  # 光源的方位角度，弧度值
    depth = 10.
    grad = np.gradient(a)  # 取图像灰度的梯度值
    grad_x, grad_y = grad  # 分别取横纵图像梯度值
    grad_x = grad_x * depth / 100.
    grad_y = grad_y * depth / 100.
    dx = np.cos(vec_el) * np.cos(vec_az)  # 光源对X轴的影响
    dy = np.cos(vec_el) * np.sin(vec_az)  # 光源对Y轴的影响
    dz = np.sin(vec_el)  # 光源对Z轴的影响
    A = np.sqrt(grad_x ** 2 + grad_y ** 2 + 1.)
    uni_x = grad_x / A
    uni_y = grad_y / A
    uni_z = 1. / A
    a1 = 255 * (dx * uni_x + dy * uni_y + dz * uni_z)  # 光源归一化
    a1 = a1.clip(0, 255)
    im1 = Image.fromarray(a1.astype('uint8'))  # 重构图像
    return im1

def make_p(i):
    print('共' + str(s) + '张,现在是第' + str(k) + '张')
    out = 'py_' + os.path.basename(i)
    make_rp(i).save(ps_out+out)

def face():
    pf = str(input('输入文件夹路径:(-1结束)'))
    if pf == '-1':
        return '-1'
    return pf


if __name__ == '__main__':
    pf = '1'
    while pf != '-1':
        pf = face()
        if pf=='-1':
            break
        if os.path.exists(pf):
            if pf[-1] != '\\':
                pf = pf + '\\'
            pfs = glob.glob(pf + '*.jpg') + glob.glob(pf + '*.png') + glob.glob(pf + '*.gif') + glob.glob(pf + '*.jpeg')
            ps_out = pf + 'py_out\\'
            s = len(pfs)
            k = 1
            if not os.path.exists(ps_out):
                os.mkdir(ps_out)
            for i in pfs:
                make_p(i)
                k += 1
            print('-' * 20 + '完成(from 杜章定1163829941)')
        else:
            print('文件夹不存在！！')
