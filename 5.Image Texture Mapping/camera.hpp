#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.hpp"

//相机类
class camera{
    public:
        camera(
            point3 lookfrom,        //相机所在位置
            point3 lookat,          //相机视线指向的位置
            vec3 vup,               //始终指向世界坐标行正上方的基向量
            double vfov,            //field of view 垂直视角
            double aspect_ratio,    //屏幕宽高比
            double aperture,        //光圈大小
            double focus_dist,      //焦距
            double t0 = 0, double t1 = 0){      //快门打开的时间
            
            auto theta = degrees_to_radians(vfov);
            auto half_h = tan(theta/2) * focus_dist;        //屏幕半高=焦距 × tan（垂直视角/2）
            auto viewport_height = 2.0 * half_h;            //屏幕总高
            auto viewport_width = aspect_ratio * viewport_height;   //屏幕总宽

            //定义摄像机的坐标系
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup,w));
            v = cross(w,u);
            
            //将图像坐标转换为摄像机坐标
            origin = lookfrom;
            //以屏幕左下角为初始点
            lower_left_corner = origin - viewport_width/2 * u - viewport_height/2 * v - focus_dist * w;


            horizontal = viewport_width * u;
            vertical = viewport_height * v;
            
            //光圈半径
            lens_radius = aperture / 2;

            time0 = t0;
            time1 = t1;
        }

        ray get_ray(double s, double t) const {

            //从镜头射出光线
            vec3 rd = lens_radius * random_in_unit_disk();
            //光线所在位置
            vec3 offset = u * rd.x() + v * rd.y();
            
            //让相机在time0到time1时间内随机生成射线
            return ray(origin + offset, 
                        lower_left_corner + s * horizontal + t * vertical - origin - offset, random_double(time0, time1));
        }

    private:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
        vec3 u, v, w;

        double lens_radius;

        //快门打开关闭时间
        double time0, time1;

};

#endif