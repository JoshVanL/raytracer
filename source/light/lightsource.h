#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include <typeinfo>
#include <vector>

#include <glm/glm.hpp>
#include "ray.h"
#include "intersection.h"

#define PI           3.14159265358979323846

using glm::vec3;

class LightSource{
public:

    glm::vec4 position;
    glm::vec3 color;
    float power;
    LightSource(glm::vec4 pos, glm::vec3 col, float pow) :
        position(pos),color(col), power(pow) {

    };
    LightSource(){
        position    = glm::vec4(0, -0.5, -1.4, 1.0);
        color       = glm::vec3(1,1,1);
        power       = 5.f;
    }


    virtual vec3 lightAtPosition(Intersection& point, const std::vector<Shape2D*>& shapes) = 0;

    virtual vec3 getDirectLight(Intersection& point, const std::vector<Shape2D*>& shapes) = 0;

    virtual vec3 getIndirectLight() = 0;

    virtual bool isOccluded(Intersection& point, const std::vector<Shape2D*>& shapes, float& transparency) = 0;

};

#endif
