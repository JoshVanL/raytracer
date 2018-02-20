#ifndef TRANSLUCENT_H
#define TRANSLUCENT_H
#include "material.h"
#include <glm/gtx/normal.hpp>
#include "../Shapes/shape2D.h"
#include "../Shapes/sphere.h"
#include "../Light/ray.h"
#include <typeinfo>
using glm::dot;
using glm::vec3;

class Translucent : public Material{
public:
    float transparency, refractive_index;
    int count = 0;
    Translucent(float transparency, float refractive_index): 
        transparency(transparency), refractive_index(refractive_index){

    }
 
    virtual vec3 material_color(const Intersection& intersection, const Ray& primary_ray, const std::vector<Shape2D*>& shapes)  override {
        Shape2D* shape2D = intersection.shape2D;   

        glm::vec3 normal = glm::normalize(shape2D->getnormal(primary_ray.position, intersection.direction));
        glm::vec3 ray_dir = (vec3) glm::normalize(intersection.direction); 
       
        float cosi = glm::dot(normal, ray_dir);
        float etai = 1, etat = 1.3; //ior. TODO: Change this value for different materials?
        if (cosi > 0) { std::swap(etai, etat); }

        float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
        
        float kr; 
        if (sint >= 1) { 
            kr = 1;
        }
        else {
            float cost = sqrtf(std::max(0.f, 1 - sint * sint));
            cosi = fabsf(cosi);
            float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
            float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
            kr = (Rs * Rs + Rp * Rp) / 2;
        }
        // vec3 refraction = refract(primary_ray, intersection.position, intersection.direction, ikntersection.shape2D, shapes, 0);
        // return refraction;
       
       /* vec3 reflection = reflect(ray, intersection.position, intersection.direction, intersection.shape2D, shapes, 0);
        
        return glm::mix(refraction, reflection, kr);
    */}

    // vec3 reflect(const Ray& originalRay, vec4 ray_orig, const vec4 ray_dir, Shape2D* t_shape, const std::vector<Shape2D*>& shapes, int depth){
    //     Intersection reflected_intersection; 
    //     vec4 reflected_dir = reflect_direction(ray_orig, ray_dir, t_shape);
    //     Ray reflected_ray(ray_orig, vec3(1,1,1), 0);
    //     count+=depth;
    //     if(reflected_ray.ClosestIntersection(reflected_dir, shapes, reflected_intersection, t_shape)){
    //             Ray outgoing_ray(reflected_intersection.position + reflected_dir*0.02f, originalRay.color, originalRay.power);
    //             return reflected_intersection.compute_color(outgoing_ray, shapes);
    //     } else {
    //         return vec3(0,0,0);
    //     }
    // }

    vec3 refract(const Ray& primary_ray, Intersection intersection, 
                Shape2D* t_shape, const std::vector<Shape2D*>& shapes){

        int currentdepth = primary_ray.bounces;
        if(currentdepth >= primary_ray.max_bounces)
            return vec3(0,0,0);

        vec4 refracted_dir = refract_direction(intersection.position, primary_ray.direction, 
                t_shape);
        Ray refracted_ray(intersection.position, refracted_dir, currentdepth + 1);

        Intersection refracted_intersection;
        if(refracted_ray.ClosestIntersection(shapes, refracted_intersection)){
            return refracted_intersection.compute_color(refracted_ray, shapes);
        }
        else {
            return vec3(0,0,0);
        }
    } 
 
    // vec4 reflect_direction(const vec4 ray_orig, const vec4 ray_dir, Shape2D* t_shape){
    //     vec4 incident_ray = -ray_dir;
    //     vec3 temp = t_shape->getnormal(ray_orig, ray_dir);
    //     vec4 normal(temp.x, temp.y, temp.z, 1);
    //     return 2.0f * dot( incident_ray, normal) * normal - incident_ray;
    // }

    vec4 refract_direction(const vec4 ray_orig, const vec4 ray_dir, Shape2D* t_shape){

        vec3 normal_3d = normalize(t_shape->getnormal(ray_orig, ray_dir));
        vec3 incoming_3d = normalize(vec3(ray_dir));
  
        // cos(theta_1) = -(N . i)
        float a = -dot(normal_3d, incoming_3d); 

        // According to Wikipedia, if dot product is negative, flip normal and recalculate.
        // This captures when the light is going out of the glass (when angle
        // between normal and refracted ray is larger than 90).
        if (a < 0) {
            normal_3d = -normal_3d;
            a = -dot(normal_3d, incoming_3d);
        }

        // 1 - cos(Θ)^2
        float b = 1 - a * a;
        // r^2 * (1 - cos(Θ)^2)
        float c = refractive_index * refractive_index * b;
        // sqrt(1 - r^2 * (1 - cos(Θ)^2))
        float d = sqrt(1 - c);
        // r * cos(Θ)
        float e = refractive_index * a;
        // N * ((r * cos(Θ)) - (sqrt(1 - r^2 * (1 - cos(Θ)^2))))
        vec3 f = (e - d) * normal_3d;
        // rI + N * ((r * cos(Θ)) - (sqrt(1 - r^2 * (1 - cos(Θ)^2))))
        vec3 g = refractive_index * incoming_3d + f;
    
        return vec4(g.x,g.y,g.z,1);
    }
    
    //this->s1->color(intersection.position, , incoming, scene, light);

   //this->s2->color(intersection.position, prim, incoming, scene, light);


};

#endif 
