#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>
#include <math.h>
#include <omp.h>

#include "material.h"
#include "../scene/SDLauxiliary.h"


using glm::vec2;
using glm::vec3;
using glm::vec4;

class Texture : public Material{

public:
    Shape2D* shape = NULL;
    vec3 zero_zero;
    vec3 max_max;
    vec2 dimensions;
    int origformat;
    unsigned char * pixels;
    int width, height;

    int specular_exponent = 100;
    float Kd = 0.8; // diffuse weight
    float Ks = 0.3; // specular weight

    Texture(const char* imagePath) : Material("Texture") {
#pragma omp critical
        {

        FILE* f = fopen(imagePath, "rb");
        unsigned char info[54];
        fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

        // extract image height and width from header
        width = *(int*)&info[18];
        height = *(int*)&info[22];

        width  = width % 3000;

        int size = 3 * width * height;
        pixels = new unsigned char[size]; // allocate 3 bytes per pixel
        fread(pixels, sizeof(unsigned char), size, f); // read the rest of the data at once
        fclose(f);
        }
    };

    vec3 get_pixel(const int x, const int y) {
        return vec3(pixels[3 * (x * width + y) + 2], pixels[3 * (x * width + y) + 1], pixels[3 * (x * width + y)]);
    }

    virtual glm::vec3 material_color(const int posx, const int posy) override {

        int i = posx % height;
        int j = posy % width;

        vec3 pixel = get_pixel(i, j);

        pixel /= vec3(255, 255, 255);

        return pixel;
    };

};

#endif
