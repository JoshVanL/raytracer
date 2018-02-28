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
        vec3 l = (vec3) intersection.direction;
        vec3 norm = intersection.shape2D->getnormal(intersection.position - intersection.direction);
        float prop = dot(normalize(norm), -l);
        float projection_factor = std::max(prop, 0.0f);

        return intersection.shape2D->color * (projection_factor * lightSource->lightAtPosition(intersection, shapes) *  intersection.shape2D->gloss);
    }

    float modulo(const float &f){
        float fp = f - std::floor(f); 
        return fp;
    }
};

#endif