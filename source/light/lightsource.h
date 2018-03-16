#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include <typeinfo>
#include <vector>
#include "../rendering/pixel.h"
#include <glm/glm.hpp>

#define PI           3.14159265358979323846

using glm::vec3;

class LightSource{
public:

    glm::vec4 position;
    glm::vec3 color;
    float depthBuffer[1400][1400];
    float power;
    LightSource(glm::vec4 pos, glm::vec3 col, float pow) :
        position(pos),color(col), power(pow) {

    };
    LightSource(){
        position    = glm::vec4(0, 0.5, -0.7, 1.0);
        color       = glm::vec3(1,1,1);
        power       = 5.f;
    }

   

};

#endif
