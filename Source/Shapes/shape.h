#ifndef SHAPE_H
#define SHAPE_H

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/normal.hpp>

#define INFINITY_ 1e8 

class Shape {
public:
    glm::vec3 center,surfaceColor, emissionColor;
    float transparency, reflection; 
    Shape(glm::vec3 centre, glm::vec3 surfaceColor, glm::vec3 emissionColor,float transparency,float reflection):
        center(center), surfaceColor(surfaceColor), emissionColor(emissionColor), transparency(transparency), reflection(reflection) {
    };
    virtual ~Shape() = 0;
    virtual bool intersect(const glm::vec3 &rayorig, const glm::vec3 &raydir, float &t0, float &t1) const {
        return false;
    }
    static float mix(const float &a, const float &b, const float &mix) 
    { 
        return b * mix + a * (1 - mix); 
    };
};
Shape::~Shape(){}

#endif