/*
Author: Ishika Soni 
Resource Used: Ray Tracing in One Weekend 
Program: A simple brute-force path tracer  
*/

#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center; 
    auto a = r.direction().length_squared(); 
    auto half_b = dot(oc, r.direction()); 
    auto c = oc.length_squared() - radius * radius; 
    auto discriminant = half_b * half_b - *a*c; 

    if (discriminant < 0) {
        return -1.0; 
    } else {
        return (-half_b - sqrt(discriminant)) / a; 
    }
}

color ray_color(const ray& r) {
    auto t = hit_sphere(point3(0, 0, -1), 0.5, r); 
    if (t > 0.0) {
        vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1)); 
        return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1); 
    }

    vec3 unit_direction = unit_vector(r.direction()); 
    t = 0.5 * (unit_direction.y() + 1.0); 
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {
    
    // Image 
    // Width and height of the image that will be outputted 
    const auto aspect_ratio = 16.0 / 9.0; 
    const int image_width = 400; 
    const int image_height = static_cast<int>(image_width / aspect_ratio); 

    // Camera 

    auto viewport_height = 2.0; 
    auto viewport_width = aspect_ratio * viewport_height; 
    auto focal_length = 1.0; 

    auto origin = point3(0, 0, 0); 
    auto horizontal = vec3(viewport_width, 0, 0); 
    auto vertical = vec3(0, viewport_height, 0); 
    auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length); 

    // Render 

    // std::cout is the written output of the image, it begins with:
    std::cout << "P3/n" << image_width << ' ' << image_width << "\n255\n"; 

    // A for loop that starts with 255 (the height - 1) and decrements by 1
    for (int j = image_height - 1; j >= 0; --j) {
        
        // Progress indicator: tells us how many lines remaining after each decrement 
        // of the loop 
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        
        // Another loop (nested for loop) that starts with 0 and increments till the 
        // width of the image, 255
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width - 1);
            auto v = double(j) / (image_height - 1);
            ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color); 

            // // R (red), g (green), and b (blue) [blue only stays at 0.25] start to 
            // // decrement and their shades change slowly since the decrement is only by 1 

            // // Int variables for each rgb auto variable; they are multiplied by 255.999 
            // // which is the image width and height and then casted as an int

            // // Image output: this will also be the written output of the image, not only 
            // // the image itself 
        }
    }
    // This loop goes from up to down and from left to right due to the ways the loops start 
    // and end 

    // Progress indicator: the program is finished running 
    std::cerr << "\nDone.\n";
    // std::cerr is used for normal written output in the terminal since std::cout is being 
    // used for the image output 
}