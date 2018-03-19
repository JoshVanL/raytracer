#ifndef SPHERE_H
#define SPHERE_H

#include <vector>

#include "shape2D.h"

#define MAX_RAY_DEPTH_ 5
#define M_PI_ 3.141592653589793

using glm::vec3;
using glm::vec4;
using glm::normalize;
using glm::dot;

using namespace std;

class Sphere : public Shape2D {
public:

    glm::vec4 center;
    float radius, radius2;

    Sphere(glm::vec4 cent, float r, glm::vec3 col) : center(cent), radius(r), radius2(r * r), Shape2D(col){

    }
        
    virtual glm::vec3 getcolor()  override {
        return color;
    }
    virtual glm::vec3 getnormal(vec4 start, vec4 dir) override {
        // glm::vec4 intersectpoint;
        // if(intersect(start, dir, intersectpoint)){
        //     vec3 intersectDir = (vec3) intersectpoint - (vec3) start;
        //     vec3 intersectNormal = intersectDir - (vec3) center;
        //     return glm::normalize(intersectNormal);
        // }
        return vec3(0,0,0);
    }
    virtual glm::vec4 toworldcoordinates(vec4 cam_intersect) override{
        return cam_intersect;
    }

    virtual vec3 minPosition() override {
        return vec3{center.x - radius, center.y - radius, center.z - radius};
    }
    virtual vec3 maxPosition() override {
        return vec3{center.x + radius, center.y + radius, center.z + radius};
    }
    virtual vec3 midpoint() override {
        return vec3{center.x, center.y, center.z};
    }


private:
    virtual bool isEqual(const Shape2D& other) const override {
        Sphere othersphere = static_cast<const Sphere&>(other);
        if(center == othersphere.center && radius == othersphere.radius)
            return true;
        return false;
    }

};
#endif
