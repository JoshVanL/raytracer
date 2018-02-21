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

    virtual glm::vec3 material_color(const Intersection& intersection, const Ray& primary_ray, const std::vector<Shape2D*>& shapes) override {
        return recurse_ray(primary_ray, intersection, intersection.shape2D, shapes, &Reflective::reflect_direction, *(this));
    }

    //Returns the color of final ray intersection point
    vec3 recurse_ray(const Ray& primary_ray, const Intersection intersection, 
                 Shape2D* t_shape, const std::vector<Shape2D*>& shapes, 
                 vec4 (Reflective::*direction_function)(const vec4, const vec4, Shape2D*),
                 Reflective& callerObj) {

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
};

#endif