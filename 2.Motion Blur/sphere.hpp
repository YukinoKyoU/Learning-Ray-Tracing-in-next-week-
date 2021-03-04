#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.hpp"
#include "vec3.hpp"

class sphere : public hittable {
    public:
        sphere() {};
        sphere(const point3& cen, const double r, shared_ptr<material> m)
                : center(cen), radius(r), mat_ptr(m){};

        virtual bool hit(const ray&r, double t_min, double t_max, hit_record& rec) const override;

    public:
        point3 center;
        double radius;
        shared_ptr<material> mat_ptr;
};

//在求交时加入摄入面的判断
bool sphere::hit(const ray&r, double t_min, double t_max, hit_record& rec) const{
    //t^2*b*b + 2*t*b*(A-C) + (A-C)*(A-C) - r^2 = 0
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = dot(oc, oc) - radius * radius;

    auto discriminant = half_b * half_b - a * c;
    if(discriminant < 0) {return false;}

    auto sqrtd = std::sqrt(discriminant);

    auto root = (-half_b - sqrtd) / a;
    //在有交点的情况下，选取较近的交点。
    if (discriminant > 0) {
        auto root = sqrt(discriminant);
        auto temp = (-half_b - root) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;

            //交点
            rec.p = r.at(rec.t);
            //获取到法线
            vec3 outward_normal = (rec.p - center) / radius;
            //设置我们想要的表面法线，即法线始终指向射线
            rec.set_face_normal(r, outward_normal);

            //材质指针
            rec.mat_ptr = mat_ptr;

            return true;
        }
        temp = (-half_b + root) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;

            //交点
            rec.p = r.at(rec.t);
            //获取到法线
            vec3 outward_normal = (rec.p - center) / radius;
            //设置我们想要的表面法线，即法线始终指向射线
            rec.set_face_normal(r, outward_normal);

            //材质指针
            rec.mat_ptr = mat_ptr;

            return true;
        }
    }
    return false;
}
#endif