#ifndef COLOR_H
#define COLOR_H

#include "vec3.hpp"
#include <iostream>

//在每次迭代中添加全色，然后在写出颜色时在末尾执行一次除法
void write_color(unsigned char *data, int image_width, int image_height, int i, int j, color pixel_color, int samples_per_pixel){
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    //将颜色除以样本数量，并进行gamma矫正，gamma=2.0
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);
    
    //写下每个颜色分量转换成[0,255]后的值
    data[(image_height-1-j) * image_width * 3 + i * 3]  =    char(256 * clamp(r, 0.0, 0.999));
    data[(image_height-1-j) * image_width * 3 + i * 3 + 1] = char(256 * clamp(g, 0.0, 0.999));
    data[(image_height-1-j) * image_width * 3 + i * 3 + 2] = char(256 * clamp(b, 0.0, 0.999));

}
#endif