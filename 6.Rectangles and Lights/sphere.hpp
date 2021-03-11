#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.hpp"
#include "vec3.hpp"

class sphere : public hittable {
    public:
        sphere() ;
        sphere(const point3& cen, const double r, shared_ptr<material> m)
                : center(cen), radius(r), mat_ptr(m){};

        virtual bool hit(const ray&r, double t_min, double t_max, hit_record& rec) const override;
        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

    public:
        point3 center;
        double radius;
        shared_ptr<material> mat_ptr;
    
    private:
        //获取以圆点为中心的单位球面上的点的uv值
        static void get_sphere_uv(const point3& p, double& u,  double& v){
            // p: 以圆点为中心的单位球面上的任意一点.
            // u: 从X = -1返回围绕Y轴的角度的值[0,1].
            // v: 从Y = -1到Y = + 1的角度的返回值[0,1].
            //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
            //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
            //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

            auto theta = acos(-p.y());
            auto phi = atan2(-p.z(),p.x()) + pi;

            u = phi / (2*pi);
            v = theta / pi;
        }
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
    if(root < t_min || t_max < root){
        root = (-half_b + sqrtd) / a;
        if(root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    //交点
    rec.p = r.at(rec.t);
    //获取到此时球心所在位置的法线
    vec3 outward_normal = (rec.p - center) / radius;
    //设置我们想要的表面法线，即法线始终指向射线
    rec.set_face_normal(r, outward_normal);

    //更新击中点并记录此坐标的uv坐标
    get_sphere_uv(outward_normal, rec.u, rec.v);

    //材质指针
    rec.mat_ptr = mat_ptr;

    return true;
}

bool sphere::bounding_box(double time0, double time1, aabb& output_box) const{

    //能包围球体的最小盒子
    output_box = aabb(
        center - vec3(radius, radius, radius),
        center + vec3(radius, radius, radius)
    );
    return true;
}

#endif