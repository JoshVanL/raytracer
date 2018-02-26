#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "lightsource.h"
#include "ray.h"

using glm::vec4;
using glm::vec3;

class SpotLight : public LightSource {

public:

    vec4 direction;
    float angleSpan;

    SpotLight(  const vec4& pos         = vec4(0, -0.5, -1.4, 1.0),
                const vec4& dir         = vec4(0, -1, 0, 0),
                const float& angle      = 0.3f,
                const glm::vec3& col    = vec3(1, 1, 1),
                const float& pow        = 10.f)
    : LightSource(pos, col, pow), direction(dir), angleSpan(angle){

    };

    virtual vec3 lightAtPosition(Intersection& point, const std::vector<Shape2D*>& shapes) override {
        vec3 directlight    = getDirectLight(point, shapes);
        vec3 indirectlight  = getIndirectLight();
        return (directlight + indirectlight);
    }

    virtual vec3 getDirectLight(Intersection& point, const std::vector<Shape2D*>& shapes) override {

        vec3 dir = glm::normalize((vec3) position - (vec3) point.position);
        vec3 norm = glm::normalize((vec3) direction);

        float uv = glm::dot(dir, norm);
        float umag = sqrt(pow(dir.x,2.f) + pow(dir.y, 2.f) + pow(dir.z, 2.f));
        float vmag = sqrt(pow(norm.x,2.f) + pow(norm.y, 2.f) + pow(norm.z, 2.f));

        float angleToPoint = fabs(acos(uv/(umag* vmag)));
        if(angleToPoint > angleSpan){
            // printf("out of angle %f \n", angleToPoint);
             return vec3(0.0001, 0.0001, 0.0001);
        }

        float dist = glm::distance(position, point.position);

        vec3 surfaceNormal =  point.shape2D->getnormal(point.position, point.direction);
        vec3 lightToPoint =   glm::normalize((vec3) point.position - (vec3) position);

        float dotProduct = glm::dot(surfaceNormal, lightToPoint);
        float powPerSurface = (power * std::max(dotProduct, 0.f))/(4 * PI * pow(dist, 2));

        float transparency = 1.f;

        if (isOccluded(point, shapes, transparency)) {
            if(transparency == 0.f)
                return vec3(0.0001, 0.0001, 0.0001);
            else {
                vec3 light_a = (color * powPerSurface * (0.8f/(transparency)));
                return (light_a * fabs(log2f( (angleToPoint + 0.01f)*2.f)) )  ;
            }
        }

        vec3 light_a = (color * powPerSurface * (0.8f/(transparency)));
        return (light_a * fabs(log2f( (angleToPoint + 0.01f)*2.f)) );
    }


    virtual bool isOccluded(Intersection& point, const std::vector<Shape2D*>& shapes, float& transparency) override {
        Intersection intersect; 
        // vec4 shadow_dir = position - point.position;
        // Ray shadow_ray(position, shadow_dir);
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
        return vec3(0.4,0.3,0.28);
    }

};

#endif
