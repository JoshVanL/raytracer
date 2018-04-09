#ifndef PORTAL_H
#define PORTAL_H
#include "material.h"
#include <glm/gtx/normal.hpp>
#include "../shapes/shape2D.h"
#include "../shapes/rectangle.h"
#include "../shapes/triangle.h"
#include "../light/ray.h"
#include <typeinfo>
#include <math.h>
#include <stdio.h>
using glm::dot;
using glm::vec3;
using glm::vec4;
class Portal : public Material {
public:

    float area;
    Triangle* triA;
    Triangle* triB;
    Triangle* triC;
    Triangle* triD;
    Portal(   Triangle* triA,
            Triangle* triB,
            Triangle* triC,
            Triangle* triD)
        :   triA(triA), triB(triB),
            triC(triC), triD(triD), Material("Portal")
    {

    }
    virtual glm::vec3 material_color(   Intersection& intersection, 
                                        const Ray& primary_ray, 
                                        const std::vector<Shape2D*>& shapes, 
                                        LightSource* lightSource,
                                        vec3 directLight,
                                        vec3 indirectLight)
    {
        if(primary_ray.bounces > primary_ray.max_depth)
            return intersection.shape2D->color; 
  
        std::pair<Ray, Triangle*> new_ray = GetNextRay(intersection, primary_ray);
        Ray raya = new_ray.first;
        Triangle* tri = new_ray.second; 
        raya.bounces += 1;
        
        Intersection new_intersection;
        if(raya.ClosestIntersection(shapes,new_intersection,tri )){
            return new_intersection.shape2D->getcolor(new_intersection, raya, shapes, lightSource);
        }
        
        return intersection.shape2D->color;
    }; 

    std::pair<Ray, Triangle*> GetNextRay(Intersection& intersect, const Ray& original_ray)
    {
        if(intersect.shape2D == triA || intersect.shape2D == triB ){
            if(intersect.shape2D == triA){
                vec4 toTriC = TransformTriSpace(triA, triC, intersect);
                Ray new_ray(toTriC, original_ray.direction);
                return std::make_pair(new_ray, triC);
            }
            else if(intersect.shape2D == triB){
                vec4 toTriD = TransformTriSpace(triB, triD, intersect);
                Ray new_ray(toTriD, original_ray.direction);      
                return std::make_pair(new_ray, triD);
            }
        }
        if(intersect.shape2D == triC || intersect.shape2D == triD ){
            if(intersect.shape2D == triC){
                vec4 toTriA = TransformTriSpace(triC, triA, intersect);
                 Ray new_ray(toTriA, original_ray.direction);
                return std::make_pair(new_ray, triA);
            }
            else if(intersect.shape2D == triD){
                vec4 toTriB = TransformTriSpace(triD, triB, intersect);
                 Ray new_ray(toTriB, original_ray.direction);
                return std::make_pair(new_ray, triB);
            }
        }
        printf("failed\n");
    }
    
    glm::vec4 TransformTriSpace(Triangle* from_tri, Triangle* to_tri, Intersection& intersect)
    {
        glm::vec2 uv = from_tri->getUV(intersect);
        vec4 e1 =   to_tri->v1 - to_tri->v0;
        vec4 e2 =   to_tri->v2 - to_tri->v0;
        return      to_tri->v0 + uv.x * e1 + uv.y * e2;
    }

};

#endif 
