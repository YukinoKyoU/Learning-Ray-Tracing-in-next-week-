#ifndef AABB_H
#define AABB_H

#include "rtweekend.hpp"

class aabb{
    public:
        aabb(){}
        aabb(const point3& a, const point3& b){
            minimum = a;
            maximum = b;
        }

        point3 min(){
            return minimum;
        }
        point3 max(){
            return maximum;
        }

        bool hit(const ray& r, double t_enter, double t_exit) const;

    public:
        //包围盒的射入边界和射出边界
        point3 minimum;
        point3 maximum;
};

//计算光线是否与包围盒相交
inline bool aabb::hit(const ray&r, double t_enter, double t_exit) const {
    for(int a = 0; a < 3; ++a){
        //t=(x-a)/b
        auto invD = 1.0f / r.direction()[a];
        //t0为光线射入包围盒的时刻
        //t1为光线射出包围盒的时刻
        auto t0 = (minimum[a] - r.origin()[a]) * invD;
        auto t1 = (maximum[a] - r.origin()[a]) * invD;

        //如果r从负方向射入，则计算的t0和t1将会交换
        if(invD < 0.0f){
            std::swap(t0, t1);
        }

        //只有光线进入3个对立面才算真正进入，出任何一个对立面就算射出
        //射入的时间应取射入（3个对立面）时间中的最大值(保证射入了所有对立面)
        //射出的时间应取射出（3个对立面）时间中的最小值
        t_enter = t0 > t_enter ? t0 : t_enter;
        t_exit = t1 < t_exit ? t1 : t_exit;
        
        //射入的时间最大值 小于 射出的时间的最小值，则光线和区域一定有交点
        if(t_enter <= t_exit)
            return false;
    }
    return true;
}

aabb surrounding_box(aabb box0, aabb box1){

    //最小边界由最小的点控制
    point3 small(fmin(box0.min().x(), box1.min().x()),
                 fmin(box0.min().y(), box1.min().y()),
                 fmin(box0.min().z(), box1.min().z()));

    //最大边界由最大的点控制
    point3 big(fmax(box0.max().x(), box1.max().x()),
               fmax(box0.max().y(), box1.max().y()),
               fmax(box0.max().z(), box1.max().z()));

    return aabb(small,big);
}

#endif