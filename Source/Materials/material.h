#ifndef MATERIAL_H
#define MATERIAL_H
#include <glm/glm.hpp>

class Shape2D;

class Material {
public:

    Material(){

    };
    virtual glm::vec3 material_color(Shape2D* shape2D) = 0;
 
};


#endif