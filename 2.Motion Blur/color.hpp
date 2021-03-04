#ifndef COLOR_H
#define COLOR_H

#include "vec3.hpp"
#include <iostream>

//handle the multi-sampled color computation
//在每次迭代中添加全色，然后在写出颜色时在末尾执行一次除法
void write_color(unsigned char *data, int image_width, int image_height, int i, int j, color pixel_color, int samples_per_pixel){
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    //Divide the color by the number of samples.
    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    //write the translated [0,255] value of each color component
    //static_cast<int> 安全转为int型
    // out << static_cast<int> (256 * clamp(r, 0.0, 0.999)) << ' '
    //     << static_cast<int> (256 * clamp(g, 0.0, 0.999)) << ' '
    //     << static_cast<int> (256 * clamp(b, 0.0, 0.999)) << '\n';
    
    data[(image_height-1-j) * image_width * 3 + i * 3]  =    char(256 * clamp(r, 0.0, 0.999));
    data[(image_height-1-j) * image_width * 3 + i * 3 + 1] = char(256 * clamp(g, 0.0, 0.999));
    data[(image_height-1-j) * image_width * 3 + i * 3 + 2] = char(256 * clamp(b, 0.0, 0.999));

}
#endif