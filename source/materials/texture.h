#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>
#include <math.h>
#include <omp.h>
#include <unistd.h>

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

        if( access( imagePath, F_OK ) == -1 ) {
            fprintf(stderr, "image file doesn't exist: %s\n", imagePath);
            exit(1);
        }

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

    virtual glm::vec3 material_color(vec3 pos3d, vec3 minPos, vec3 maxPos) override {

        float percentX, percentY;

        if (minPos.x == maxPos.x) {
            percentX = (pos3d.z - minPos.z) / (maxPos.z - minPos.z);
        } else {
            percentX = (pos3d.x - minPos.x) / (maxPos.x - minPos.x);
        }
        if (minPos.y == maxPos.y) {
            percentY = (pos3d.z - minPos.z) / (maxPos.z - minPos.z);
        } else {
            percentY = (pos3d.y - minPos.y) / (maxPos.y - minPos.y);
        }

        if (isinf(percentX) || isnan(percentX)) {
            percentX = 0;
        }
        if (isinf(percentY) || isnan(percentY)) {
            percentY = 0;
        }

        if (percentX < 0) {
            percentX = -percentX;
        }
        if (percentY < 0) {
            percentY = -percentY;
        }

        float i = abs(percentX * ((float) height));
        float j = abs(percentY * ((float) width));

        if (i < 0) {
            i = -i;
        }
        if (j < 0) {
            j = -j;
        }


        i = fmod(i, height);
        j = fmod(j, width);

        vec3 pixel = get_pixel(i, j);

        pixel /= vec3(255, 255, 255);

        return pixel;
    };

};

#endif
