#ifndef WAVE_H
#define WAVE_H

#include "shape2D.h"
#include "../light/intersection.h"
#include <glm/gtx/norm.hpp>
#include <glm/gtx/normal.hpp>
#include "../light/ray.h"
class Wave: public Shape2D{
public:

    vec4 v0, v1;
    vec4 center;
    Wave(vec4 v0, vec4 v1, vec3 color, std::string id, const std::initializer_list<Material*>& materials = std::initializer_list<Material*>())
    : v0(scalevec4(v0)), v1(scalevec4(v1)), center(scalevec4(vec4(  (v0.x + v1.x)/2.f, (v0.y + v1.y)/2.f, (v0.z + v1.z)/2.f , 1.f )   )),
    Shape2D(color, materials, "Wave")
    {

    };
    virtual glm::vec3 getcolor(Intersection& intersection, const Ray& primary_ray, const std::vector<Shape2D*>& shapes, LightSource* lightSource){
        return color;
    };

    virtual glm::vec4 toworldcoordinates(glm::vec4 cam_intersect){
        // float u = cam_intersect[1], v = cam_intersect[2];
        // vec4 e1 = v1 - v0;
        // vec4 e2 = v2 - v0;
        // return v0 + u * e1 + v * e2;
    };

    virtual vec2 getUV(Intersection& intersectpoint){
        vec3 norm = glm::normalize(glm::cross(vec3(v0) - vec3(v0.x,v1.y,v0.z), vec3(v0.x,v1.y,v0.z) - vec3(v1)));
        vec3 u = normalize(vec3( norm.y, -norm.x, 0)); // Assuming that a != 0 and b != 0, otherwise use c.
        vec3 v = cross(norm, u); // If n was normalized, v is already normalized. Otherwise normalize it.
        float u_coord = dot(u,(vec3)intersectpoint.position);
        float v_coord = dot(v,(vec3)intersectpoint.position);
        return glm::vec2(u_coord, v_coord);
    }

    virtual bool intersect(Ray& ray, glm::vec3 dir, glm::vec4& intersectionpoint) 
    { 
        vec3 e1 = vec3(v0) - vec3(center);
        vec3 e2 = vec3(v1) - vec3(center);
        vec3 norm = glm::normalize(glm::cross(e1, e2));
        if(glm::dot(norm, (vec3)ray.direction) > 0)
            norm = -norm;
        float denom = dot(norm, (vec3)ray.direction);
        std::cout << norm.x  << "\n";
        if (abs(denom) > 0.0001f) // your favorite epsilon
        {
            float t = dot(vec3(center - ray.position),vec3(norm) ) / denom;
            if (t >= 0){
                printf("yay\n");
                return true;
            } 
        }
        return false;

    } 
    virtual vec3 getnormal(Intersection& intersection){
       
        vec3 e1 = vec3(v0) - vec3(center);
        vec3 e2 = vec3(v1) - vec3(center);
        vec3 norm = glm::normalize(glm::cross(e1, e2));
        if(glm::dot(norm, (vec3)intersection.direction) <= 0)
            return norm;
        else 
            return -norm;
    }
     virtual bool isEqual(const Shape2D& other) const {
        return true;
    }
};


#endif