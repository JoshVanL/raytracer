#ifndef SHAPE_H
#define SHAPE_H

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/normal.hpp>

#define INFINITY_ 1e8 

class Shape3D {
public:
    glm::vec3 center,surfaceColor, emissionColor;
    float transparency, reflection; 
    Shape3D(glm::vec3 centre, glm::vec3 surfaceColor, glm::vec3 emissionColor,float transparency,float reflection):
        center(center), surfaceColor(surfaceColor), emissionColor(emissionColor), transparency(transparency), reflection(reflection) {
    };
    virtual ~Shape3D() = 0;
    static float mix(const float &a, const float &b, const float &mix) 
    { 
        return b * mix + a * (1 - mix); 
    };
};
Shape3D::~Shape3D(){}

#endif