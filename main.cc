/*
Author: Ishika Soni 
Resource Used: Ray Tracing in One Weekend 
Program: A simple brute-force path tracer  
*/

#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

#include <iostream>

color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec; 

    // If we've exceeded the ray bounce limit, no more light is gathered. 
    if (depth <= 0) {
        return color(0, 0, 0);
    }

    // Calculating reflected ray origins with tolerance 
    if (world.hit(r, 0.001, infinity, rec)) {
        // Replacement diffuse 
        point3 target = rec.p + random_in_hemisphere(rec.normal); // Hemispherical scattering 
        return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth-1); 
    }
    vec3 unit_direction = unit_vector(r.direction()); 
    auto t = 0.5 * (unit_direction.y() + 1.0); 
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {
    
    // Image 
    // Width and height of the image that will be outputted 
    const auto aspect_ratio = 16.0 / 9.0; 
    const int image_width = 400; 
    const int image_height = static_cast<int>(image_width / aspect_ratio); 
    const int samples_per_pixel = 100; 
    const int max_depth = 50; 

    // World 
    hittable_list world; 
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100)); 

    // Camera 
    camera cam; 

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
            color pixel_color(0, 0, 0); 

            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1); 
                auto v = (j + random_double()) / (image_height - 1); 
                ray r = cam.get_ray(u, v); 

                // ray_color() with depth limiting 
                pixel_color += ray_color(r, world, max_depth); 
            }

            // Rendering with multi-sampled pixels 
            write_color(std::cout, pixel_color, samples_per_pixel); 

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