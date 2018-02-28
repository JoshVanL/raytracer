#ifndef SPECULAR_H
#define SPECULAR_H

#include "material.h"
#include "diffuse.h"

using glm::vec3;
using glm::normalize;

class Specular : public Material {

public:
    int specular_exponent = 100;
    Diffuse* diffuse_shader;
    float Kd = 0.8; // diffuse weight 
    float Ks = 0.3; // specular weight 

    Specular(): diffuse_shader(new Diffuse()), Material(){

    };

    virtual glm::vec3 material_color(Intersection& intersection, const Ray& primary_ray, const std::vector<Shape2D*>& shapes, LightSource* lightSource){
         //Calculate reflection ray direction
        vec3 l = (vec3) (lightSource->position - intersection.position);
        vec3 surface_normal = normalize(intersection.shape2D->getnormal(intersection));

        vec3 reflected_dir = normalize(2.0f * dot(l, surface_normal) * surface_normal - l);

        //Viewing direction
        vec3 v = normalize(-vec3(primary_ray.direction));

        //Calculate component of viewing direction in the direction of reflected ray
        float specular_highlight = dot(v, reflected_dir);
        float new_specular_highlight = glm::pow(specular_highlight, specular_exponent);

        vec3 intensity = lightSource->getDirectLight(intersection, shapes);
        vec3 indirectlight = lightSource->getIndirectLight();
        /* Calculating Specular Component */
        vec3 specular_component = new_specular_highlight * intensity * vec3(1,1,1);

        /* Calculating Diffuse Component */
        vec3 diffuse_component = diffuse_shader->material_color(intersection, primary_ray, shapes, lightSource);

        return specular_component * Ks +  diffuse_component * Kd ;
    }

};

#endif
