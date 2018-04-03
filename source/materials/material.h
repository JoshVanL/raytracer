#ifndef MATERIAL_H
#define MATERIAL_H
#include <glm/glm.hpp>
#include <iterator>
#include <algorithm>
#include <vector>
#include "material_properties/gloss.h"
class Shape2D;
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


    virtual glm::vec3 material_color(const int posx, const int posy) = 0;

};


#endif
