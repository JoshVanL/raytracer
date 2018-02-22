#ifndef SOLID_H
#define SOLID_H
#include "material.h"
#include "../Shapes/shape2D.h"
#include "../Light/intersection.h"
#include "../Light/lightsource.h"

class Solid : public Material{
public:
    Solid(): Material(0.f){

    };

    virtual glm::vec3 material_color(Intersection& intersection, const Ray& primary_ray, const std::vector<Shape2D*>& shapes, LightSource& lightSource) override {
        glm::vec3 flat_color = intersection.shape2D->color;
        glm::vec3 light = lightSource.lightAtPosition(intersection, shapes);
        glm::vec3 final_color = flat_color * (light * intersection.shape2D->gloss);
        return final_color;
    }

 
}; 


#endif