#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include <glm/glm.hpp>


class LightSource{
public:
    glm::vec4 position;
    glm::vec3 color;
    float power;
    LightSource(glm::vec4 pos, glm::vec3 col, float pow): position(pos), color(col), power(pow){};
    LightSource(){
        position      = glm::vec4(0, -0.5, -1.4, 1.0);
        color         = glm::vec3(1,1,1);
        power         = 10.f;
    };
};

#endif