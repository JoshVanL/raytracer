#ifndef DIFFUSE_H
#define DIFFUSE_H
#include "../Light/lightsource.h"
#include "material.h"
using glm::vec3;
using glm::vec4;

class Diffuse : public Material {

public:
    Diffuse(): Material(){

    }

    virtual vec3 material_color(Intersection& intersection, const Ray& primary_ray, const std::vector<Shape2D*>& shapes, LightSource* lightSource) override {
        vec3 l = (vec3) ((lightSource->position - intersection.position));
        vec3 norm = intersection.shape2D->getnormal(intersection.position);
        // The proportion of light hitting the surface.
        float prop = dot(normalize(norm), l);
        // Because negative light is not allowed.
        float projection_factor = std::max(prop, 0.0f);

        return projection_factor * lightSource->lightAtPosition(intersection, shapes) * intersection.shape2D->color;
    }

    float modulo(const float &f){
        float fp = f - std::floor(f); 
        return fp;
    }
};

#endif