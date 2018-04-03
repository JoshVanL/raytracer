#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>
#include <math.h>
#include <omp.h>

#include "diffuse.h"
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
    Diffuse* diffuse_shader;
    float Kd = 0.8; // diffuse weight
    float Ks = 0.3; // specular weight

    Texture(const char* imagePath) : Material("Texture") {
#pragma omp critical
        {
        diffuse_shader = new Diffuse();

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

    virtual glm::vec3 material_color(Intersection& intersection, const Ray& primary_ray, const std::vector<Shape2D*>& shapes, LightSource* lightSource,
                                        vec3 directLight,
                                        vec3 indirectLight) override {

        vec3 l = (vec3) (lightSource->position - intersection.position);
        vec3 surface_normal = normalize(intersection.shape2D->getnormal(intersection));

        vec3 reflected_dir = normalize(2.0f * dot(l, surface_normal) * surface_normal - l);

        //Viewing direction
        vec3 v = normalize(vec3(primary_ray.direction));

        //Calculate component of viewing direction in the direction of reflected ray
        float specular_highlight = dot(v, reflected_dir);
        float new_specular_highlight = glm::pow(specular_highlight, specular_exponent);

        /* Calculating Specular Component */
        vec3 specular_component = new_specular_highlight * directLight * vec3(1,1,1);

        /* Calculating Diffuse Component */
        vec3 diffuse_component = diffuse_shader->material_color(intersection, primary_ray, shapes, lightSource,
                                         directLight,
                                         indirectLight);
        float j;
        float i;

        if(intersection.position.x < 1.f && intersection.position.x > -0.1) {
            i = abs((((intersection.position.x ) ) * (float) height));
        } else {
            i = abs((((intersection.position.z ) ) * (float) height));
        }

        if(intersection.position.y < 1.f && intersection.position.y > -0.1) {
            j = abs((((intersection.position.y ) ) * (float) width));
        } else {
            j = abs((((intersection.position.z ) ) * (float) width));
        }

        i = fmod(i, height);
        j = fmod(j, width);

        vec3 pixel = get_pixel(i, j);

        pixel /= vec3(255, 255, 255);

        return (specular_component * Ks +  diffuse_component * Kd) * pixel * vec3(0.8, 0.8, 0.8);
    };

};

#endif
