#ifndef DIFFUSE_H
#define DIFFUSE_H
#include "../Light/lightsource.h"
#include "material.h"
using glm::vec3;
using glm::vec4;

class Diffuse : public Material {

public:
    Diffuse(): Material(){

    }

    virtual vec3 material_color(Intersection& intersection, const Ray& primary_ray, const std::vector<Shape2D*>& shapes, LightSource& lightSource) override {
        glm::vec3 lightIntensity = lightSource.lightAtPosition(intersection, shapes);
        glm::vec3 lightDir = (vec3) intersection.position - (vec3) lightSource.position;
        // compute the pattern
        float angle = (45) * (PI / 180); 
        float s = intersection.position.x - intersection.position.y ; 
        float t = intersection.position.y + intersection.position.x ; 
        float scaleS = 20, scaleT = 20; 

        float pattern = (modulo(s * scaleS) < 0.5); 
        vec3 hitNormal = -intersection.shape2D->getnormal(intersection.position);

        return intersection.shape2D->color* lightIntensity * std::max(0.f, glm::dot(hitNormal, -lightDir)); 
    }

    float modulo(const float &f){
        float fp = f - std::floor(f); 
        return fp;
    }
};

#endif