#ifndef SOLID_H
#define SOLID_H
#include "material.h"
#include "../Shapes/shape2D.h"
class Solid : public Material{

    Solid(){

    };

    virtual glm::vec3 material_color(Shape2D* shape2D) override {
        return shape2D->color;
    }
   
}; 


#endif