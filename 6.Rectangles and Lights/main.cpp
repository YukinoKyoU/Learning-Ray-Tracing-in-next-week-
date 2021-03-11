#include "rtweekend.hpp"

#include "color.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"
#include "material.hpp"
#include "moving_sphere.hpp"
#include "bvh.hpp"

#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


//地球贴图场景
hittable_list earth() {
    auto earth_texture = make_shared<image_texture>("earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point3(0,0,0), 2, earth_surface);

    return hittable_list(globe);
}

//创建柏林噪声的场景
hittable_list two_perlin_spheres(){
    hittable_list objects;

    //创建指向柏林噪声材质的指针
    auto pertext = make_shared<noise_texture>(4);

    objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
    objects.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

    return hittable_list(make_shared<bvh_node>(objects, 0, 1));

}

//创建两个球的场景
hittable_list two_spheres(){
    hittable_list objects;

    auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));

    objects.add(make_shared<sphere>(point3(0, -10, 0), 10, make_shared<lambertian>(checker)));
    objects.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));

    return hittable_list(make_shared<bvh_node>(objects, 0, 1));

}

hittable_list random_scene(){
    hittable_list world;

    //将大球设置为棋盘
    auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));

    for(int a = -11; a < 11; a++){
        for(int b = -11; b < 11; b++){
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if((center - point3(4, 0.2, 0)).length() > 0.9){
                shared_ptr<material>sphere_material;

                if(choose_mat < 0.8){
                    //diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);

                    auto center2 = center + vec3(0, random_double(0, .5), 0);
                    world.add(make_shared<moving_sphere>(center, center2, 0.0, 1.0, 0.2, sphere_material));

                }else if(choose_mat < 0.95){
                    //metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }else{
                    //glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return hittable_list(make_shared<bvh_node>(world, 0.0, 1.0));
}


color ray_color(const ray&r, const hittable& world, int depth) {
    hit_record rec;
    //递归深度
    if(depth <= 0){
        return color(0, 0, 0);
    }

    if(world.hit(r, 0.001, infinity, rec)){
        ray scattered;
        color attenuation;
        //如果发生了漫反射或者全反射
        if(rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            //以反射后的光线进行递归
            return attenuation * ray_color(scattered, world, depth-1);
        return color(0, 0, 0);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main(){
    //设置图像参数
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int> (image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    //设置场景,使用switch语句，以针对给定的运行选择所需的场景
    hittable_list world;

    point3 lookfrom;
    point3 lookat;
    auto vfov = 40.0;
    auto aperture = 0.0;

    switch (0) {
        case 1:
            world = random_scene();
            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            vfov = 20.0;
            aperture = 0.1;
            break;

        case 2:
            world = two_spheres();
            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            vfov = 20.0;
            break;

        case 3:
            world = two_perlin_spheres();
            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            vfov = 20.0;
            break;

        default:
        case 4:
            world = earth();
            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            vfov = 20.0;
            break;
    }


    //设置相机
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;

    //将参数传入相机，
    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

    //渲染
    unsigned char* data = new unsigned char[image_width * image_height * 3];

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for(int j = 0; j < image_height; ++j){
        for(int i = 0; i < image_width ; ++i){
            
            color pixel_color(0, 0, 0);
            for(int s = 0; s < samples_per_pixel; ++s){
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(data, image_width, image_height, i, j, pixel_color, samples_per_pixel);  
        }
    }

    stbi_write_jpg("output.jpg", image_width, image_height, 3, data, 100);

}