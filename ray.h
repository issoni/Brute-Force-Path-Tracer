#ifndef RAY_H
#define RAY_H

#include "vec3.h"

// P(t) = A + t*b
// P: 3D position along a line in 3D 
// A: ray origin 
// b: ray direction 
// t: a real number (negative values: go anywhere on the 3D lines, positive values: only parts in front of A, a half-line or ray)
// Plug in different t and P(t) moves point along the ray 
class ray {
    public: 
        ray() {}
        ray(const point3& origin, const vec3& direction) 
            : orig(origin), dir(direction) {} 
        
        point3 origin() const {
            return orig;
        }

        vec3 direction() const {
            return dir;
        }

        point3 at(double t) const {
            return orig + (t * dir); 
        }
    
    public: 
        point3 orig; 
        vec3 dir; 
}; 

#endif