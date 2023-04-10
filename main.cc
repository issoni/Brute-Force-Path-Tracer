/*
Author: Ishika Soni 
Resource Used: Ray Tracing in One Weekend 
Program: A simple brute-force path tracer  
*/
#include "color.h"
#include "vec3.h"

#include <iostream>

int main() {
    
    // Image 

    // Width and height of the image that will be outputted 
    const int image_width = 256; 
    const int image_height = 256; 

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

            color pixel_color(double(i) / (image_width - 1), double(j) / (image_height - 1), 0.25);
            write_color(std::cout, pixel_color); 

            // // R (red), g (green), and b (blue) [blue only stays at 0.25] start to 
            // // decrement and their shades change slowly since the decrement is only by 1 
            // auto r = double(i) / (image_width - 1); 
            // auto g = double(j) / (image_height - 1); 
            // auto b = 0.25; 

            // // Int variables for each rgb auto variable; they are multiplied by 255.999 
            // // which is the image width and height and then casted as an int 
            // int ir = static_cast<int>(255.999 * r); 
            // int ig = static_cast<int>(255.999 * g);
            // int ib = static_cast<int>(255.999 * b); 

            // // Image output: this will also be the written output of the image, not only 
            // // the image itself 
            // std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
    // This loop goes from up to down and from left to right due to the ways the loops start 
    // and end 

    // Progress indicator: the program is finished running 
    std::cerr << "\nDone.\n";
    // std::cerr is used for normal written output in the terminal since std::cout is being 
    // used for the image output 
}