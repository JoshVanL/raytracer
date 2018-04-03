#include <typeinfo>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/normal.hpp>
#include "../materials/material.h"

using glm::vec3;
using glm::vec4;

#ifndef SHAPE2D_H
#define SHAPE2D_H

class Ray;
class Material;
class Intersection;
class Shape3D;

class Shape2D{
public:
    const float L = 1000;// Length of Cornell Box side.

    vec3    color;
    vec4    normal;
    Material* mat = nullptr;
    Shape2D(vec3 color): color(color){

    }

    vec4 scalevec4(vec4 v) {
        v *= 2/L;
        v -= vec4(1,1,1,1);
        v.x *= -1;
        v.y *= -1;
        v.w = 1.0;
        return v;
    }
    vec3 scalevec3(vec3 v) {
        v *= 2/L;
        v -= vec3(1,1,1);
        v.x *= -1;
        v.y *= -1;
        return v;
    }
    float scalefloat(float v) {
        v *= 2/L;
        return v;
    }
    virtual glm::vec3 getcolor(vec3 pos3d) = 0;
    virtual glm::vec4 toworldcoordinates(glm::vec4 cam_intersect) = 0;
    virtual glm::vec3 getnormal(glm::vec4 start, glm::vec4 dir) = 0;

    virtual void SetMaterial(Material* material) {
        material = material;
    }

    virtual vec3 ComputeNormal() {
        return vec3();
    }

    virtual vec3 minPosition() {
        return vec3();
    };
    virtual vec3 maxPosition() {
        return vec3();
    };
    virtual vec3 midpoint() {
        return vec3();
    };

    virtual std::vector<vec4> verticies() {
        return std::vector<vec4>();
    }

    bool operator==(const Shape2D& other) const
    {
        // If the derived types are the same then compare them
        return typeid(*this) == typeid(other) && isEqual(other);
    }
private:
    virtual bool isEqual(const Shape2D& other) const = 0;
};

#endif
