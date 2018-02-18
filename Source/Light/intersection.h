#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "../Shapes/triangle.h"
#include <glm/glm.hpp>
class Intersection {
public:
    glm::vec4 position;
    float distance;
    int triangleIndex;
    Triangle* triangle;
    Intersection(){};
};


#endif