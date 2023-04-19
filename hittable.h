#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "rtweekend.h"

class material; 

struct hit_record {
    point3 p; 
    vec3 normal; 
    shared_ptr<material> mat_ptr; 
    double t; 
    bool front_face; 

    // Adds front-face tracking to hit_record (normals always point "outward" from the surface or against the incident ray)
    // @r: ray that is being sent 
    // @outward_normal: which way the normal will point 
    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0; 
        normal = front_face ? outward_normal :-outward_normal; 
    }
}; 

// Abstract class for anything a ray might hit and make both sphere and list of spheres just something you can hit 
class hittable {
    public: 
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0; 

}; 

#endif