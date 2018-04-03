#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include "lightsource.h"
#include <glm/gtx/simd_vec4.hpp> 
using glm::vec4;
using glm::vec3;

class PointLight : public LightSource {
public:

    PointLight( const vec4& pos         = vec4(0.5, 0.2, -0.5, 1.0), 
                const glm::vec3& col    = vec3(1, 1, 1), 
                const float& pow        = 50.f) 
    : LightSource(pos, col, pow){

    };

    virtual vec3 lightAtPosition(Intersection& point, const std::vector<Shape2D*>& shapes) override {
        vec3 directlight    = getDirectLight(point, shapes);
        vec3 indirectlight  = getIndirectLight(); 
        return (directlight + indirectlight);
    }

    virtual vec3 getDirectLight(Intersection& point, const std::vector<Shape2D*>& shapes) override {
        vec4 point_position = point.shape2D->id == "Terrain" ? scalevec4(point.position) : point.position;
        float dist = glm::distance(position, point_position);
        vec3 surfaceNormal =  point.shape2D->getnormal(point);
        vec3 pointToLight =   glm::normalize((vec3) position - (vec3) point_position);
        float dotProduct = glm::dot(surfaceNormal, pointToLight);
        float powPerSurface = (power * std::max(dotProduct, 0.f))/(4 * PI * pow(dist, 2));
        
        // if (isOccluded(point, shapes)) {
        //     vec3 shadow(0.0005, 0.0005, 0.0005);
        //     return shadow  * color;
        // }
        // printf("%f %f %f \n", point.position.x, point.position.y, point.position.z );
        return color * powPerSurface;
    }

    virtual bool isOccluded(Intersection& point, const std::vector<Shape2D*>& shapes) override {
        vec4 point_position = point.shape2D->id == "Terrain" ? scalevec4(point.position) : point.position;
        Intersection intersect; 
        vec4 shadow_dir = glm::normalize(point_position - position);
        Ray shadow_ray(position + shadow_dir * 2.f, shadow_dir);
        if(shadow_ray.ClosestIntersection(shapes, intersect, "Smoke"/*, point.shape2D*/)){
            float distA = glm::distance(point_position, position);
            float distB = glm::distance(scalevec4(intersect.position), position);

            return (distB < distA);
        }
        return false;
    }

    virtual vec3 getIndirectLight() override {
        return vec3(0.55,0.3,0.23);
    }
    vec4 scalevec4(vec4 v) {
        v *= 2/1000;
        v -= vec4(1,1,1,1);
        v.x *= -1;
        v.y *= -1;
        v.w = 1.0;
        return v;
    }
};



#endif
