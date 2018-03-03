#ifndef MATERIAL_H
#define MATERIAL_H
#include <glm/glm.hpp>
#include <iterator>
#include <algorithm>
#include <vector>
#include "MaterialProperties/gloss.h"
class Shape2D;
class Intersection;
class Ray;
using glm::vec4;
using glm::vec3;

class Material {
public:
    const float transparency;
    const std::string material_type;
    const Gloss* gloss;
    Material(std::string material_type, 
             GlossType glossType = HIGHGLOSS, 
             const float& trans = 0.f) : 

             gloss(new Gloss(glossType)), material_type(material_type), transparency(trans){

    };
    virtual glm::vec3 material_color(Intersection& intersection, const Ray& primary_ray, const std::vector<Shape2D*>& shapes, LightSource* lightSource) = 0;
 
};


#endif 