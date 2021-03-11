#ifndef BVH_H
#define BVH_H

#include "rtweekend.hpp"

#include "hittable.hpp"
#include "hittable_list.hpp"

#include <algorithm>

class bvh_node : public hittable{
    public:
        bvh_node();

        bvh_node(const hittable_list& list, double time0, double time1)
            : bvh_node(list.objects, 0, list.objects.size(), time0, time1){}

        bvh_node(
            const std::vector<shared_ptr<hittable>> & src_objects,
            size_t start, size_t end, double time0, double time1);

        virtual bool hit(const ray&r, double t_min, double t_max, hit_record& rec) const override;
        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

    public:
        //子节点指针是hittable，这个指针可以指向所有的hittable类。例如bvh_node， sphere
        shared_ptr<hittable> left;
        shared_ptr<hittable> right;
        aabb box;
};

//sort（）使用的比较函数。先判断是哪个轴，然后按对应轴升序排列
inline bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis){
    aabb box_a;
    aabb box_b;

    if(!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b)){
        std::cerr << "No bounding box in bvh_node constructor.\n";
    }
    return box_a.min().e[axis] < box_b.min().e[axis];
}

bool box_x_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b){
    return box_compare(a, b, 0);
}

bool box_y_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b){
    return box_compare(a, b, 1);
}

bool box_z_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b){
    return box_compare(a, b, 2);
}

bvh_node::bvh_node(
    const std::vector<shared_ptr<hittable>>& src_objects,
    size_t start, size_t end, double time0, double time1
    ){
        //创建原场景对象可修改的数组
        auto objects = src_objects;

        //随机选取一个轴
        int axis = random_int(0,2);
        //comparator为函数指针
        auto comparator = (axis == 0) ? box_x_compare
                         :(axis == 1) ? box_y_compare
                                      : box_z_compare;

        //物体个数
        size_t object_span = end - start;

        //两种特殊情况（物体数量为1或者2）
        if(object_span == 1){
            //若只有一个物体，则左右节点都指向该物体
            left = right = objects[start];
        }else if(object_span == 2){
            //若有2个物体，直接左右节点分别指向物体
            if(comparator(objects[start], objects[start + 1])){ 
                left = objects[start];
                right = objects[start + 1];
            }else{
                left = objects[start + 1];
                right = objects[start];
            }
        }else{
            //对所有物体进行排序，以该轴正方向的顺序
            std::sort(objects.begin() + start, objects.begin() + end, comparator);

            //对半分，每个子树分一半的物体个数
            auto mid = start + object_span/2;
            //进行左右子数的递归包围盒
            left = make_shared<bvh_node>(objects, start, mid, time0, time1);
            right = make_shared<bvh_node>(objects, mid, end, time0, time1);

        }

        aabb box_left, box_right;
        
        //最后将当前节点的aabb设置为包含左右子数的box，并判断物体是否有包围盒
        if(!left->bounding_box(time0, time1, box_left) || !right->bounding_box(time0, time1, box_right)){
            std::cerr << "No bounding box in bvh_node constructor.\n";
        }
        box = surrounding_box(box_left, box_right);

}

//检查这个节点的box是否被击中，如果击中，就对这个节点的子节点进行判断（二叉树结构）
bool bvh_node::hit(const ray&r, double t_min, double t_max, hit_record& rec) const{
    if(!box.hit(r, t_min, t_max))
        return false;

    bool hit_left = left->hit(r, t_min, t_max, rec);
    bool hit_right = right->hit(r, t_min, hit_left ? rec.t : t_max, rec);

    return hit_left || hit_right;
}

bool bvh_node::bounding_box(double time0, double time1, aabb& output_box) const{

    output_box = box;
    return true;
}

#endif