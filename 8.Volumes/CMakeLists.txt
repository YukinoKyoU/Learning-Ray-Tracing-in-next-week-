cmake_minimum_required(VERSION 3.10)
project(Volumes)

# find_package(OpenCV REQUIRED)
find_package(OpenMP REQUIRED)
if(OPENMP_FOUND)
    message("Found OpenMP")
    set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS})
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS})
    set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS})
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Release")


include_directories(/usr/local/include)

add_executable(Volumes main.cpp aabb.hpp camera.hpp color.hpp hittable_list.hpp hittable.hpp ray.hpp 
                material.hpp rtweekend.hpp sphere.hpp vec3.hpp moving_sphere.hpp stb_image_write.h 
                bvh.hpp texture.hpp perlin.hpp stb_image.h aarect.hpp box.hpp constant_medium.hpp )
