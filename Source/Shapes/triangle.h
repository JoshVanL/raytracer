#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <glm/glm.hpp>
#include "shape2D.h"
using glm::vec3;
using glm::vec4;

class Triangle : public Shape2D
{
public:
    glm::vec4 v0;
    glm::vec4 v1;
    glm::vec4 v2;
    glm::vec4 normal;
    glm::vec3 color;
    glm::vec3 gloss;

    Triangle( glm::vec4 v0, glm::vec4 v1, glm::vec4 v2, glm::vec3 color, glm::vec3 gloss)
        : v0(v0), v1(v1), v2(v2), color(color), gloss(gloss)
    {
        ComputeNormal();
    }

    bool operator== (Triangle& other) const {
        if( (v0 == other.v0 || v0 == other.v1 || v0 == other.v2) && 
            (v1 == other.v0 || v1 == other.v1 || v1 == other.v2) &&
            (v2 == other.v0 || v2 == other.v1 || v2 == other.v2))
            return false;
        return true;
    }

    void ComputeNormal()
    {
        glm::vec3 e1 = glm::vec3(v1.x-v0.x,v1.y-v0.y,v1.z-v0.z);
        glm::vec3 e2 = glm::vec3(v2.x-v0.x,v2.y-v0.y,v2.z-v0.z);
        glm::vec3 normal3 = glm::normalize( glm::cross( e2, e1 ) );
        normal.x = normal3.x;
        normal.y = normal3.y;
        normal.z = normal3.z;
        normal.w = 1.0;
    }
};
#endif