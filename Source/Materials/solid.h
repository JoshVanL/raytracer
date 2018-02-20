#ifndef SOLID_H
#define SOLID_H
#include "material.h"
#include "../Shapes/shape2D.h"
#include "../Light/intersection.h"

class Solid : public Material{
public:
    Solid(): Material(0.f){

    };

    virtual glm::vec3 material_color(const Intersection& intersection, const Ray& primary_ray, const std::vector<Shape2D*>& shapes) override {
        return intersection.shape2D->color;
    }

 
}; 


#endif