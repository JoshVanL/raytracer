#ifndef MATERIAL_H
#define MATERIAL_H
#include <glm/glm.hpp>
#include <vector>
class Shape2D;
class Intersection;
class Ray;
using glm::vec4;
using glm::vec3;

class Material {
public:
    const float transparency;
    Material(const float& trans = 0.f) : transparency(trans){

    };
    virtual glm::vec3 material_color(Intersection& intersection, const Ray& primary_ray, const std::vector<Shape2D*>& shapes, LightSource& lightSource) = 0;
 
    vec4 reflect_direction(const vec4 ray_orig, const vec4 ray_dir, Shape2D* t_shape){
        vec4 incident_ray = -ray_dir;
        vec3 temp = t_shape->getnormal(ray_orig, ray_dir);
        vec4 normal(temp.x, temp.y, temp.z, 1);
        return 2.0f * dot( incident_ray, normal) * normal - incident_ray;
    }


};


#endif 