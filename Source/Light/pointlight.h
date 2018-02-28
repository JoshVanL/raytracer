#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include "lightsource.h"

using glm::vec4;
using glm::vec3;

class PointLight : public LightSource {
public:

    PointLight( const vec4& pos         = vec4(0, -0.5, -1.4, 1.0), 
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

        vec3 surfaceNormal =  point.shape2D->getnormal(point.position);
        vec3 lightToPoint =   glm::normalize((vec3) point.position - (vec3) position);

        float dotProduct = glm::dot(surfaceNormal, lightToPoint);
        if(point.shape2D->name == "Sphere"){
            dotProduct = glm::dot(-surfaceNormal, lightToPoint);
        }   
        float powPerSurface = (power * std::max(dotProduct, 0.f))/(4 * PI * pow(dist, 2));

        float transparency = 1.f;
        if (isOccluded(point, shapes, transparency)) {
            return vec3(0.0001, 0.0001, 0.0001);

        }

        return color * powPerSurface;
    }

    virtual bool isOccluded(Intersection& point, const std::vector<Shape2D*>& shapes, float& transparency) override {
        Intersection intersect; 
        vec4 shadow_dir = point.position - position;
        Ray shadow_ray(position + shadow_dir*0.01f, shadow_dir);
        if(shadow_ray.ClosestIntersection(shapes, intersect, point.shape2D)){
            float distA = glm::distance(point.position, position);
            float distB = glm::distance(intersect.position, position);
            if(distB < distA){
                if(intersect.shape2D->material != nullptr)
                    transparency = intersect.shape2D->material->transparency;
                else 
                    transparency = 0.f;
                return true;
            }
        }
        return false;
    }

    virtual vec3 getIndirectLight() override {
        return vec3(0.2,0.1,0.09);
    }

};



#endif