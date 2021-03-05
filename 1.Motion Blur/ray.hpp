#ifndef RAY_H
#define RAY_H

#include "vec3.hpp"

class ray{
    public:
        ray() {};

        //每条光线都能储存自己所在的时刻
        ray(const point3& origin, const vec3& direction, double time= 0.0): orig(origin), dir(direction), tm(time)
        {}

        vec3 origin() const { return orig; }
        vec3 direction() const { return dir; }
        double time() const {return tm;}

        vec3 at(double t) const {
            return orig + t * dir;
        }

    public:
        vec3 orig;
        vec3 dir;
        double tm;
};

#endif