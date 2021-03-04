#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.hpp"


class material;

//将所有需要的参数打包在一起。
//当光线摄入一个表面，hitrecord的材质指针会被物体表面的材质指针所赋值。
struct hit_record {
    point3 p;
    vec3 normal;
    shared_ptr<material> mat_ptr;
    double t;

    //记录是否为正面
    bool front_face;

    inline void set_face_normal(const ray&r, const vec3& outward_normal){
        //点积小于0 ray outside 点积大于0 ray inside
        front_face = dot(r.direction(), outward_normal) < 0;
        //始终让法线指向射线，即让法线和射线的点积始终小于0
        normal = front_face ? outward_normal : -outward_normal;
    }
};
//任何能和光线求交的东西实现时都继承这个类
class hittable{
    public:
        //区间tmin和tmax来判断相交是否有效
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};


#endif