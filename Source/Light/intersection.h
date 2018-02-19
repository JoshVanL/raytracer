#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <glm/glm.hpp>
// #include "../Shapes/triangle.h"
#include "../Shapes/shape2D.h"

class Intersection {
public:
    glm::vec4 position, direction;
    float distance;
    int triangleIndex;
    Shape2D* shape2D;
    // Triangle* triangle;
    // Sphere* sphere;
    Intersection(glm::vec4 pos, glm::vec4 dir, float dis, Shape2D* shape2D): position(pos), direction(dir), distance(dis), shape2D(shape2D){

    };
    // Intersection(glm::vec4 pos, float dis, Sphere* sph): position(pos), distance(dis), sphere(sph){

    // };
    Intersection(){

    };
};


#endif