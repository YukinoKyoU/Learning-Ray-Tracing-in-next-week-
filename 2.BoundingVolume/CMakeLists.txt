cmake_minimum_required(VERSION 3.10)
project(BVH)

# find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/local/include)

add_executable(BVH main.cpp aabb.hpp camera.hpp color.hpp hittable_list.hpp hittable.hpp ray.hpp 
                material.hpp rtweekend.hpp sphere.hpp vec3.hpp moving_sphere.hpp stb_image_write.h bvh.hpp)
