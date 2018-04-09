#ifndef LUMINOUS_H
#define LUMINOUS_H

#include "material.h"
#include "diffuse.h"

using glm::vec3;
using glm::vec4;
using glm::normalize;

class Luminous : public Material, public LightSource {

public:
    int specular_exponent = 100;
    Diffuse* diffuse_shader;
    float Kd = 0.8; // diffuse weight 
    float Ks = 0.3; // specular weight 

    Luminous(vec4 pos, vec3 col, float pow): diffuse_shader(new Diffuse()), LightSource(pos, col, pow), Material("Luminous"){
 
    };

    virtual glm::vec3 material_color(Intersection& intersection, const Ray& primary_ray, const std::vector<Shape2D*>& shapes, LightSource* lightSource,
                                        vec3 directLight,
                                        vec3 indirectLight){
        //Calculate reflection ray direction
        float scaling =  (-1.f * dot(vec3(intersection.direction), intersection.shape2D->getnormal(intersection))) + 0.00001f;

        vec3 result = intersection.shape2D->color * (1.f - scaling + 0.2f);
        return result;
    }

    virtual vec3 lightAtPosition(Intersection& point, const std::vector<Shape2D*>& shapes){
        vec3 directlight    = getDirectLight(point, shapes);
        vec3 indirectlight  = getIndirectLight(); 
        return (directlight + indirectlight);
    }

    virtual bool isOccluded(Intersection& point, const std::vector<Shape2D*>& shapes) override {
        vec4 point_position =  point.position;
        Intersection intersect; 
        vec4 shadow_dir = glm::normalize(point_position - position);
        Ray shadow_ray(position + shadow_dir * 0.05f, shadow_dir);
        Shape2D* excludedShape =point.shape2D;
        if(shadow_ray.ClosestIntersection(shapes, intersect, "Smoke", excludedShape)){
          std::vector<Material*>::iterator it = std::find_if(intersect.shape2D->materials.begin(), intersect.shape2D->materials.end(), Ray::find_material("Luminous"));
            if(it != intersect.shape2D->materials.end()){
                printf("yay\n");// << "yay\n";
                return false; 
            }
            vec4 intersect_position =  intersect.position;
            float distA = glm::distance(point_position, position);
            float distB = glm::distance(intersect_position, position);

            return (distB < distA);
        }
        return false;
    }

    virtual vec3 getDirectLight(Intersection& point, const std::vector<Shape2D*>& shapes){
        vec4 point_position =point.position;
        float dist = glm::distance(position, point_position);
        
        vec3 surfaceNormal =  -point.shape2D->getnormal(point);
        vec3 pointToLight =   glm::normalize((vec3) position - (vec3) point_position);
        float dotProduct = glm::dot(surfaceNormal, pointToLight);
        float powPerSurface = (power * std::max(dotProduct, 0.f))/(4 * PI * pow(dist, 2));
        
        if (isOccluded(point, shapes)) {
            return vec3(0.0005, 0.0005, 0.0005) * point.shape2D->color;
        }
        return color * powPerSurface;
    }

    virtual vec3 getIndirectLight(){
            return vec3(0.55,0.3,0.23);
    }

};

#endif
