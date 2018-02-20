#ifndef TRANSLUCENT_H
#define TRANSLUCENT_H
#include "material.h"
#include <glm/gtx/normal.hpp>
#include "../Shapes/shape2D.h"
#include "../Shapes/sphere.h"
#include "../Light/ray.h"
#include <typeinfo>
#include <math.h>
#include <stdio.h>
using glm::dot;
using glm::vec3;

class Translucent : public Material{
public:
    const float refractive_index;
    const float air_refractive_index;
    int count = 0;

    Translucent(const float& transparency = 1.f, const float& refractive_index = 0.6f) : 
        Material(transparency), refractive_index(refractive_index), air_refractive_index(1.f){
    }
 
    virtual vec3 material_color(const Intersection& intersection, const Ray& primary_ray, const std::vector<Shape2D*>& shapes)  override {
        Shape2D* shape2D            = intersection.shape2D;   
        glm::vec3 normal            = glm::normalize(shape2D->getnormal(primary_ray.position, primary_ray.direction));
        glm::vec3 ray_dir           = (vec3) glm::normalize(primary_ray.direction); 
       
        float cosi                  = glm::dot(normal, ray_dir);

        float incoming = air_refractive_index;
        float outgoing = refractive_index;

        if (cosi > 0) { std::swap(incoming, outgoing); }
        float cos2 = sqrtf(std::max(0.f, 1 - std::pow(cosi, 2.f)));
        float sint = (incoming / refractive_index) * cos2;
        
        float kr = 1;

        if(sint < 1){
            float cost = sqrtf(std::max(0.f, 1 - sint * sint));
            cosi = fabsf(cosi);
            float Rs = ((outgoing * cosi) - (incoming * cost)) / ((outgoing * cosi) + (incoming * cost));
            float Rp = ((incoming * cosi) - (outgoing * cost)) / ((incoming * cosi) + (outgoing * cost));
            kr = (Rs * Rs + Rp * Rp) / 2;
        }
        vec3 refraction = recurse_ray(primary_ray, intersection, intersection.shape2D, shapes, &Translucent::refract_direction, *(this));
        vec3 reflection = recurse_ray(primary_ray, intersection, intersection.shape2D, shapes, &Translucent::reflect_direction, *(this));
        return glm::mix(refraction, reflection, kr) * transparency;
    }

    vec3 recurse_ray(const Ray& primary_ray, const Intersection intersection, 
                 Shape2D* t_shape, const std::vector<Shape2D*>& shapes, 
                 vec4 (Translucent::*direction_function)(const vec4, const vec4, Shape2D*),
                 Translucent& callerObj) {

        int currentdepth = primary_ray.bounces;
        if(currentdepth >= primary_ray.max_depth)
            return vec3(0,0,0);

        vec4 new_dir = (callerObj.*direction_function)(intersection.position, primary_ray.direction,t_shape);
        Ray new_ray(intersection.position, new_dir, currentdepth + 1);

        Intersection new_intersection;
        if(new_ray.ClosestIntersection(shapes, new_intersection, t_shape)){
            return new_intersection.compute_color(new_ray, shapes);
        }
        else {
            return vec3(0,0,0);
        }
    } 
 
    vec4 reflect_direction(const vec4 ray_orig, const vec4 ray_dir, Shape2D* t_shape){
        vec4 incident_ray = -ray_dir;
        vec3 temp = t_shape->getnormal(ray_orig, ray_dir);
        vec4 normal(temp.x, temp.y, temp.z, 1);
        return 2.0f * dot( incident_ray, normal) * normal - incident_ray;
    }

    vec4 refract_direction(const vec4 ray_orig, const vec4 ray_dir, Shape2D* t_shape){

        vec3 normal_3d = normalize(t_shape->getnormal(ray_orig, ray_dir));
        vec3 incoming_3d = normalize(vec3(ray_dir));
  
        float a = -dot(normal_3d, incoming_3d); 

        if (a < 0) {
            normal_3d = -normal_3d;
            a = -dot(normal_3d, incoming_3d);
        }
        float b = 1 - a * a;
        float c = refractive_index * refractive_index * b;
        float d = sqrt(1 - c);
        float e = refractive_index * a;
        vec3 f = (e - d) * normal_3d;
        vec3 g = refractive_index * incoming_3d + f;
    
        return vec4(g.x,g.y,g.z,1);
    }

};

#endif 
