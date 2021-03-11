#ifndef AARECT_H
#define AARECT_H

#include "rtweekend.hpp"

#include "hittable.hpp"

class xy_rect : public hittable {
    public:
        xy_rect() {}

        xy_rect(double _x0, double _x1, double _y0, double _y1, double _k, 
            shared_ptr<material> mat)
            : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {};

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            // 包围盒理论上面无厚薄，但是实际中面有厚薄，我们可以将厚度设置为0.0001，以此模拟理论厚度
            output_box = aabb(point3(x0, y0, k - 0.0001), point3(x1, y1, k + 0.0001));
            return true;
        }

    public:
        shared_ptr<material> mp;
        double x0, x1, y0, y1, k;
};

bool xy_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    
    //根据已知的分量z求出t，
    auto t = (k - r.origin().z()) / r.direction().z();
    if (t < t_min || t > t_max)
        return false;
    
    //将t带入两个其他分量中
    auto x = r.origin().x() + t*r.direction().x();
    auto y = r.origin().y() + t*r.direction().y();

    //如果两个分量不在边界内，则光线没打到
    if (x < x0 || x > x1 || y < y0 || y > y1)
        return false;

    //反之，求取该点的纹理坐标，以及其他碰撞点信息
    rec.u = (x - x0) / (x1 - x0);
    rec.v = (y - y0) / (y1 - y0);
    rec.t = t;
    auto outward_normal = vec3(0, 0, 1);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;

    rec.p = r.at(t);
    
    return true;
}

#endif