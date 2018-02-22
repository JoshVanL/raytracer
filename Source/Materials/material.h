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
    
};


#endif 