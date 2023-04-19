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
#include "material.h"

#include <iostream>

hittable_list random_scene() {
    hittable_list world; 

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5)); 
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material)); 

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double(); 
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double()); 

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material; 

                if (choose_mat < 0.8) {
                    // Diffuse 
                    auto albedo = color::random() * color::random(); 
                    sphere_material = make_shared<lambertian>(albedo); 
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // Metal 
                    auto albedo = color::random(0.5, 1); 
                    auto fuzz = random_double(0, 0.5); 
                    sphere_material = make_shared<metal>(albedo, fuzz); 
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // Glass 
                    sphere_material = make_shared<dielectric>(1.5); 
                    world.add(make_shared<sphere>(center, 0.2, sphere_material)); 
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5); 
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1)); 

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1)); 
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2)); 

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world; 
}

color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec; 

    // If we've exceeded the ray bounce limit, no more light is gathered. 
    if (depth <= 0) {
        return color(0, 0, 0);
    }

    // Calculating reflected ray origins with tolerance. Ray color with scattered reflectance. 
    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered; 
        color attenuation; 
        if(rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * ray_color(scattered, world, depth - 1); 
        }
        return color(0, 0, 0); 
    }

    vec3 unit_direction = unit_vector(r.direction()); 
    auto t = 0.5 * (unit_direction.y() + 1.0); 
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}


int main() {
    
    // Image 
    // Width and height of the image that will be outputted 
    const auto aspect_ratio = 3.0 / 2.0; 
    const int image_width = 1200; 
    const int image_height = static_cast<int>(image_width / aspect_ratio); 
    const int samples_per_pixel = 500; 
    const int max_depth = 50; 

    // World 

    auto world = random_scene(); 

    /* Wide angle camera 
    auto R = cos(pi/4); 
    hittable_list world; 

    auto material_left = make_shared<lambertian>(color(0, 0, 1)); 
    auto material_right = make_shared<lambertian>(color(1, 0, 0)); 

    world.add(make_shared<sphere>(point3(-R, 0, -1), R, material_left));
    world.add(make_shared<sphere>(point3(R, 0, -1), R, material_right)); 
    */

    /* Zoomed in defocused blur
    hittable_list world; 

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0)); 
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5)); 
    auto material_left = make_shared<dielectric>(1.5); 
    auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0); 

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground)); 
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center)); 
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left)); 
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.45, material_left));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));  
    */

    // Camera 
    point3 lookfrom(13, 2, 3); 
    point3 lookat(0, 0, 0); 
    vec3 vup(0, 1, 0); 
    auto dist_to_focus = 10.0; 
    auto aperture = 0.1; 

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus); 

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