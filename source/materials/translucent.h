#ifndef TRANSLUCENT_H
#define TRANSLUCENT_H
#include "material.h"
#include <glm/gtx/normal.hpp>
#include "../shapes/shape2D.h"
#include "../light/ray.h"
#include <typeinfo>
#include <math.h>
#include <stdio.h>
using glm::dot;
using glm::vec3;

class Translucent : public Material{
public:
    const float refractive_index;
    const float air_refractive_index;
    const bool isReflective;
    const bool isRefractive;

    Translucent(const bool isReflective = true,
                const bool isRefractive = true,
                const float& transparency = 0.7f,
                const float& refractive_index = 0.7f) :
                Material("Translucent", HIGHGLOSS, transparency), refractive_index(refractive_index), air_refractive_index(1.f),
                isReflective(isReflective), isRefractive(isRefractive) {
    }

    virtual vec3 material_color(Intersection& intersection, const Ray& primary_ray, const std::vector<Shape2D*>& shapes,  LightSource* lightSource,
                                        vec3 directLight,
                                        vec3 indirectLight)  override {
        if(isRefractive || isReflective){
            Shape2D* shape2D            = intersection.shape2D;
            glm::vec3 normal            = glm::normalize((vec3) shape2D->getnormal(intersection));
            glm::vec3 ray_dir           = (vec3) glm::normalize(primary_ray.direction);

            float cosi                  = glm::dot(normal, ray_dir);

            float incoming = air_refractive_index;
            float outgoing = refractive_index;

            if (cosi > 0) { std::swap(incoming, outgoing); }
            float cos2 = sqrtf(std::max(0.f, 1 - std::pow(cosi, 2.f)));
            float sint = (incoming / refractive_index) * cos2;

            float kr = 0.4f;

            if(sint < 1){
                float cost = sqrtf(std::max(0.f, 1 - sint * sint));
                cosi = fabsf(cosi);
                float Rs = ((outgoing * cosi) - (incoming * cost)) / ((outgoing * cosi) + (incoming * cost));
                float Rp = ((incoming * cosi) - (outgoing * cost)) / ((incoming * cosi) + (outgoing * cost));
                kr = (Rs * Rs + Rp * Rp) * 0.5f;
            }

            vec3 reflection;
            vec3 refraction;

            if(isReflective){
                reflection = recurse_ray(primary_ray, intersection, intersection.shape2D, shapes, lightSource, &Translucent::reflect_direction, *(this)) +  directLight * 0.02f;
                if(!isRefractive)
                    return reflection;
            }
            if(isRefractive){
                refraction = recurse_ray(primary_ray, intersection, intersection.shape2D, shapes, lightSource, &Translucent::refract_direction, *(this)) +  directLight * 0.02f ;
                if(!isReflective)
                    return refraction;
            }
            return glm::mix(refraction, reflection, kr) * transparency;
        }
        else{
            return vec3(0,0,0);
        }
    }

    //Returns the color of final ray intersection point
    vec3 recurse_ray(const Ray& primary_ray, Intersection intersection, 
                 Shape2D* t_shape, const std::vector<Shape2D*>& shapes, 
                 LightSource* lightSource,
                 vec4 (Translucent::*direction_function)(Intersection, Shape2D*),
                 Translucent& callerObj) {

        int currentdepth = primary_ray.bounces;
        if(currentdepth >= primary_ray.max_depth)
            return vec3(0,0,0);

        vec4 new_dir = (callerObj.*direction_function)(intersection, t_shape);
        Ray new_ray(intersection.position, new_dir, currentdepth + 1);

        Intersection new_intersection;
        if(new_ray.ClosestIntersection(shapes, new_intersection, t_shape)){
            return new_intersection.shape2D->getcolor(new_intersection, new_ray, shapes, lightSource);
        }
        
        return vec3(0,0,0);
        
    } 
 
    vec4 reflect_direction(Intersection intersection, Shape2D* t_shape){
        vec4 incident_ray = -intersection.direction;
        vec3 incoming_3d = normalize(vec3(intersection.direction));
        vec3 norm3d = t_shape->getnormal(intersection);

        if (dot(norm3d, incoming_3d) > 0) {
            norm3d = -norm3d;
        }

        vec4 norm = vec4(norm3d.x,norm3d.y,norm3d.z,1);
        return (dot(incident_ray, norm) * norm) * 2.f - incident_ray;
    }

    vec4 refract_direction(Intersection intersection, Shape2D* t_shape){

        vec3 normal_3d = normalize(t_shape->getnormal(intersection));
        vec3 incoming_3d = normalize(vec3(intersection.direction));
  
        float a = dot(normal_3d, incoming_3d); 

        if (a > 0) {
            normal_3d = -normal_3d;
            a = dot(normal_3d, incoming_3d);
        }
        float d = sqrt(1 - (refractive_index * refractive_index * (1 - a * a)));
        float e = refractive_index * a;
        vec3 f = (e - d) * normal_3d;
        vec3 g = refractive_index * incoming_3d + f;
    
        return vec4(g.x,g.y,g.z,1);
    }

};

#endif 
