#ifndef TEXTURE_H
#define TEXTURE_H

#include "material.h"
#include "SDL.h"

using glm::vec3;
using glm::vec4;

class Texture : public Material{
public:
    SDL_Surface *image = nullptr;
    Shape2D* shape = nullptr;
    vec3 zero_zero;
    vec3 max_max;
    vec2 dimensions;
    Texture(std::string imagePath) : Material("Texture"), image(SDL_LoadBMP(imagePath)), dimensions(image->w,image->h){

    };

    void setup_shape(Shape2D* shape2D){
        shape = shape2D;
        zero_zero = shape->minPosition();
        max_max = shape->maxPosition();
    }

    virtual glm::vec3 material_color(Intersection& intersection, const Ray& primary_ray, const std::vector<Shape2D*>& shapes, LightSource* lightSource) override {
        Shape2D* t_shape = intersection.shape2D;
        if(shape != t_shape)
            setup_shape(t_shape);
        vec3 pos = (vec3) intersection.position;
        
    };
 
};

#endif