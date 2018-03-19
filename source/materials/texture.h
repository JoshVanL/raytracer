#ifndef TEXTURE_H
#define TEXTURE_H

#include "material.h"
#include <SDL.h>
#include <math.h>
#include "diffuse.h"
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
        diffuse_shader = new Diffuse();

        FILE* f = fopen(imagePath, "rb");
        unsigned char info[54];
        fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

        // extract image height and width from header
        width = *(int*)&info[18];
        height = *(int*)&info[22];

        width  = width % 3100;

        int size = 3 * width * height;
        pixels = new unsigned char[size]; // allocate 3 bytes per pixel
        fread(pixels, sizeof(unsigned char), size, f); // read the rest of the data at once
        fclose(f);

        for(int i = 0; i < size; i += 3) {
                unsigned char tmp = pixels[i];
                pixels[i] = pixels[i+2];
                pixels[i+2] = tmp;
        }
    };

    vec3 get_pixel(const int x, const int y) {
        return vec3(pixels[3 * (x * width + y)], pixels[3 * (x * width + y) + 1], pixels[3 * (x * width + y) + 2]);
    }

    virtual glm::vec3 material_color(Intersection& intersection, const Ray& primary_ray, const std::vector<Shape2D*>& shapes, LightSource* lightSource) override {

        vec3 l = (vec3) (lightSource->position - intersection.position);
        vec3 surface_normal = normalize(intersection.shape2D->getnormal(intersection));

        vec3 reflected_dir = normalize(2.0f * dot(l, surface_normal) * surface_normal - l);

        //Viewing direction
        vec3 v = normalize(-vec3(primary_ray.direction));

        //Calculate component of viewing direction in the direction of reflected ray
        float specular_highlight = dot(v, reflected_dir);
        float new_specular_highlight = glm::pow(specular_highlight, specular_exponent);

        vec3 intensity = lightSource->getDirectLight(intersection, shapes);
        vec3 indirectlight = lightSource->getIndirectLight();
        /* Calculating Specular Component */
        vec3 specular_component = new_specular_highlight * intensity * vec3(1,1,1);

        /* Calculating Diffuse Component */
        vec3 diffuse_component = diffuse_shader->material_color(intersection, primary_ray, shapes, lightSource);

        Shape2D* t_shape = intersection.shape2D;
        vec2 uv = t_shape->getUV(intersection);

        float i = (((intersection.position.x + 1.f) ) * (float) width);
        float j = (((intersection.position.z + 1.f) ) * (float) height);

        vec3 pixel = get_pixel(i, j);

        pixel /= vec3(255, 255, 255);
        pixel *= vec3(0.5, 0.5, 0.5);

        return (specular_component * Ks +  diffuse_component * Kd) * pixel;
    };

 

};

#endif
