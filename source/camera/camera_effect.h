#ifndef CAMERA_EFFECT_H
#define CAMERA_EFFECT_H
#include <glm/glm.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/gtx/norm.hpp>
#include <math.h>
#include <vector>
using glm::vec2;
using glm::vec3;
using glm::vec4;

class CameraEffect {
public:
    float width;
    float height;
    CameraEffect(float width, float height): width(width),height(height) {

    }
    virtual bool applyCameraEffect(std::vector<std::vector<vec3>>& srcpixels, std::vector<std::vector<vec3>>& destpixels, double SCREEN_HEIGHT, double SCREEN_WIDTH) = 0;

};


#endif 