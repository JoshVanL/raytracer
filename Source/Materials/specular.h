#ifndef SPECULAR_H
#define SPECULAR_H
#include "material.h"
#include "../Light/lightsource.h"
#include "../Light/ray.h"
#include "reflective.h"
using glm::vec4;
using glm::vec3;

class Specular : public Material {


    virtual glm::vec3 material_color(Intersection& intersection, const Ray& primary_ray, const std::vector<Shape2D*>& shapes, LightSource& lightSource) override {
        float transp = transparency;
        if(lightSource.isOccluded(intersection, shapes, transp)){
            return vec3(0,0,0);
        }

        vec3 lightAtPoint = lightSource.lightAtPosition(intersection, shapes);
        vec4 lightDir = intersection.position - lightSource.position;
        vec3 normal_light = intersection.shape2D->getnormal(lightSource.position, lightDir);
        vec3 normal_camera = intersection.shape2D->getnormal(lightSource.position, primary_ray.direction);
        Ray lightRay(intersection.position, lightDir);

        vec3 diffuse    = lightAtPoint * std::max(0.f, glm::dot( normal_light, (vec3) -lightDir)); 
        vec3 reflection = (vec3) reflect_direction(lightSource.position, lightDir, intersection.shape2D); 
        vec3 specular = /*vis **/ lightAtPoint * (float) std::pow(std::max(0.f, glm::dot(reflection, (vec3) -primary_ray.direction)), 10); 
        return diffuse * 0.8f + specular * 0.2f; 
    }     

};


#endif