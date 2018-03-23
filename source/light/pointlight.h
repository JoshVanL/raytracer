#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include "lightsource.h"
#include <glm/gtx/simd_vec4.hpp> 
using glm::vec4;
using glm::vec3;

class PointLight : public LightSource {
public:

    PointLight( const vec4& pos         = vec4(0.5, 0.5, -1.0, 1.0), 
                const glm::vec3& col    = vec3(1, 1, 1), 
                const float& pow        = 10.f) 
    : LightSource(pos, col, pow){

    };

    virtual vec3 lightAtPosition(Intersection& point, const std::vector<Shape2D*>& shapes) override {
        vec3 directlight    = getDirectLight(point, shapes);
        vec3 indirectlight  = getIndirectLight(); 
        return (directlight + indirectlight);
    }

    virtual vec3 getDirectLight(Intersection& point, const std::vector<Shape2D*>& shapes) override {
        float dist = glm::distance(position, point.position);

        vec3 surfaceNormal =  point.shape2D->getnormal(point);
        vec3 pointToLight =   glm::normalize((vec3) position - (vec3) point.position);

        float dotProduct = glm::dot(surfaceNormal, pointToLight);
        float powPerSurface = (power * std::max(dotProduct, 0.f))/(4 * PI * pow(dist, 2));

        if (isOccluded(point, shapes)) {
            return vec3(0.0001, 0.0001, 0.0001);
        }

        return color * powPerSurface;
    }

    virtual bool isOccluded(Intersection& point, const std::vector<Shape2D*>& shapes) override {
        Intersection intersect; 
        vec4 shadow_dir = glm::normalize(point.position - position);
        Ray shadow_ray(position + shadow_dir*0.01f, shadow_dir);
        if(shadow_ray.ClosestIntersection(shapes, intersect, point.shape2D)){
            float distA = glm::distance(point.position, position);
            float distB = glm::distance(intersect.position, position);
            return (distB < distA);
        }
        return false;
    }

    virtual vec3 getIndirectLight() override {
        return vec3(0.55,0.3,0.23);
    }

};



#endif
