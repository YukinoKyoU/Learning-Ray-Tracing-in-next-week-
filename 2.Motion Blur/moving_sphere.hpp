#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H

#include "rtweekend.hpp"

#include "hittable.hpp"

//移动的球类
class moving_sphere : public hittable{
    public:
        moving_sphere(){};
        moving_sphere(
            point3 cen0, point3 cen1, double _time0, double _time1, double r, shared_ptr<material>m):
            center0(cen0), center1(cen1), time0(_time0), time1(_time1), radius(r), mat_ptr(m)
            {};

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec
        ) const override;

        point3 center(double time) const;

    public:
        //球心center0和center1
        point3 center0, center1;
        //移动的时间
        double time0, time1;
        double radius;
        shared_ptr<material> mat_ptr;
};

//球心在time0到time1时间段内从center0线性运动到center1
point3 moving_sphere::center(double time) const{
    return center0 + ((time - time0)/(time1 - time0)) * (center1 - center0);
}

bool moving_sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center(r.time());

    //t^2*b*b + 2*t*b*(A-C) + (A-C)*(A-C) - r^2 = 0
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = dot(oc, oc) - radius * radius;

    auto discriminant = half_b * half_b - a * c;
    if(discriminant < 0) {return false;}

    auto sqrtd = std::sqrt(discriminant);

    auto root = (-half_b - sqrtd) / a;
    //在有交点的情况下，选取较近的交点。
    if(root < t_min || t_max < root){
        root = (-half_b + sqrtd) / a;
        if(root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    //交点
    rec.p = r.at(rec.t);
    //获取到法线
    vec3 outward_normal = (rec.p - center(r.time())) / radius;
    //设置我们想要的表面法线，即法线始终指向射线
    rec.set_face_normal(r, outward_normal);

    //材质指针
    rec.mat_ptr = mat_ptr;

    return true;
        
}


#endif