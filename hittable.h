#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

struct hit_record {
    point3 p; 
    vec3 normal; 
    double t; 
    bool front_face; 

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0; 
        normal = front_face ? outward_normal :-outward_normal; 

    // bool front_face;
    // if (dot(ray_direction, outward_normal) > 0.0) {
    //     // ray is inside the sphere
    //     normal = -outward_normal;
    //     front_face = false;
    // } else {
    //     // ray is outside the sphere
    //     normal = outward_normal;
    //     front_face = true;
    // }
    }
}; 

class hittable {
    public: 
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0; 

}; 

#endif