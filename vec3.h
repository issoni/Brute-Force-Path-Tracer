#ifndef VEC3_H
#define VEC3_H

#include <cmath> 
#include <iostream> 

using std::sqrt; 

// Used for colors, locations, directions, offsets 
// Two aliases for vec3: 'point3' and 'color'
class vec3 {
    public:
        vec3() { e[0] = 0; e[1] = 0; e[2] = 0; }
        vec3(double e0, double e1, double e2) {
            e[0] = e0;
            e[1] = e1;
            e[2] = e2;
        }

        double x() const {
            return e[0];
        }

        double y() const {
            return e[1]; 
        }

        double z() const {
            return e[2]; 
        }

        vec3 operator-() const {
            return vec3(-e[0], -e[1], -e[2]);
        }

        double operator[] (int i) const {
            return e[i]; 
        }

        double& operator[] (int i) {
            return e[i]; 
        }

        vec3& operator+=(const vec3 &v) {
            e[0] += v.e[0]; 
            e[1] += v.e[1]; 
            e[2] += v.e[2]; 
            return *this; 
        }

        vec3& operator*=(const double t) {
            e[0] *= t; 
            e[1] *= t; 
            e[2] *= t; 
            return *this; 
        }

        vec3& operator/=(const double t) {
            return *this *= 1/t; 
        }

        double length() const {
            return sqrt(length_squared()); 
        }

        double length_squared() const {
            return (e[0] * e[0]) + (e[1] * e[1]) + (e[2] * e[2]); 
        }

        inline static vec3 random() {
            return vec3(random_double(), random_double(), random_double()); 
        }

        inline static vec3 random(double min, double max) {
            return vec3(random_double(min, max), random_double(min, max), random_double(min, max)); 
        }

        // Return true if the vector is close to zero in all dimensions
        bool near_zero() const {
            const auto s = 1e-8; 
            return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s); 
        }
        

    public: 
        double e[3]; 
};

// Type aliases for vec3 (3D point and RGB color)
using point3 = vec3;  
using color = vec3;  

//vec3 Utility Functions 

inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2]; 
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]); 
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]); 
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3 &v) {
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]); 
}

inline vec3 operator*(const vec3 &v, double t) {
    return t * v; 
}

inline vec3 operator/(vec3 v, double t) {
    return (1 / t) * v; 
}

inline double dot(const vec3 &u, const vec3 &v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1], 
                u.e[2] * v.e[0] - u.e[0] * v.e[2], 
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length(); 
}

// Rejection method: pick random point in unit cube where x, y, z range from -1 to +1
// Reject this point aand try again if point is outside sphere
inline vec3 random_in_unit_sphere() {
    while(true) {
        auto p = vec3::random(-1, 1); 
        if (p.length_squared() >= 1) {
            continue; 
        }
        return p; 
    }
}

// Picks random points in the unit sphere and normalizes them 
inline vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere()); 
}

// Diffuse method that has a uniform scatter direction for all angles away from the hit point
// (with no dependence on angle from normal)
inline vec3 random_in_hemisphere(const vec3& normal) {
    vec3 in_unit_sphere = random_in_unit_sphere(); 
    if (dot(in_unit_sphere, normal) > 0.0) { // In the same hemisphere as the normal
        return in_unit_sphere; 
    } else {
        return -in_unit_sphere; 
    }
}

// v + 2b (minus in front of v since it points in)
inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v,n) * n; 
}

// Calculates a refraction ray 
inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0); 
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n); 
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n; 
    return r_out_perp + r_out_parallel; 
}

// Generate random point inside unit disk 
inline vec3 random_in_unit_disk() {
    while (true) {
        auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0); 
        if (p.length_squared() >= 1) {
            continue; 
        }
        return p; 
    }
}

#endif