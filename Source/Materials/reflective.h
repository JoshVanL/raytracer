#ifndef REFLECTIVE_H
#define REFLECTIVE_H
#include "material.h"
#include <glm/gtx/normal.hpp>
#include "../Shapes/shape2D.h"
#include "../Light/ray.h"
#include <math.h>
#include <stdio.h>
using glm::dot;
using glm::vec3;

class Reflective : public Material {
public:
    Reflective() : Material(0.f){

    };

    virtual glm::vec3 material_color(Intersection& intersection, const Ray& primary_ray, const std::vector<Shape2D*>& shapes, LightSource& lightSource) override {
        return recurse_ray(primary_ray, intersection, intersection.shape2D, shapes, lightSource, &Reflective::reflect_direction, *(this));
    }

    //Returns the color of final ray intersection point
    vec3 recurse_ray(const Ray& primary_ray, Intersection intersection, 
                 Shape2D* t_shape, const std::vector<Shape2D*>& shapes, 
                 LightSource& lightSource,
                 vec4 (Reflective::*direction_function)(const vec4, const vec4, Shape2D*),
                 Reflective& callerObj) {

        int currentdepth = primary_ray.bounces;
        if(currentdepth >= primary_ray.max_depth)
            return vec3(0,0,0);

        vec3 new_dir3d = t_shape->getnormal(intersection.position);
        vec4 new_dir = vec4(new_dir3d.x,new_dir3d.y,new_dir3d.z,1);
        Ray new_ray(intersection.position, new_dir, currentdepth + 1);

        Intersection new_intersection;
        if(new_ray.ClosestIntersection(shapes, new_intersection, t_shape)){
            return new_intersection.shape2D->getcolor(new_intersection, new_ray, shapes, lightSource);
        }
        else {
            return vec3(1,1,1);
        }
    } 
    vec4 reflect_direction(const vec4 point, const vec4 ray_dir, Shape2D* t_shape){
        
        vec3 normal3d = (vec3) t_shape->getnormal(point); //
        vec4 normal = vec4(normal3d.x, normal3d.y,normal3d.z, 1); 
        return 2.0f * dot(ray_dir, normal) * normal - ray_dir ;
    }
};

#endif