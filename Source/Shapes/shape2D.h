#ifndef SHAPE2D_H
#define SHAPE2D_H
#include <typeinfo>
#include <glm/glm.hpp>
#include <glm/gtx/normal.hpp>


class Ray;

using glm::vec3;

class Shape2D{
public:

    const float L = 555;			// Length of Cornell Box side.
    int foo;
    vec3 color, emissionColor, gloss;
    float transparency, reflection; 

    Shape2D(vec3 color, vec3 gloss, vec3 emissCol, float transp, float refl): 
        color(color), gloss(gloss), emissionColor(emissCol), transparency(transp), reflection(refl){

    };

    Shape2D(vec3 color) : color(color), gloss(vec3(0,0,0)), emissionColor(vec3(0,0,0)), transparency(0), reflection(0){

    }

    Shape2D(vec3 color, vec3 gloss) : color(color), gloss(gloss), emissionColor(vec3(0,0,0)), transparency(0), reflection(0){

    }

    virtual bool intersect(Ray& ray, glm::vec3 dir, glm::vec4& intersectionpoint) = 0;
    virtual glm::vec4 tocamcoordinates(float u, float v) = 0;
    virtual glm::vec3 getnormal() = 0;
    virtual bool operator==(const Shape2D& other) const{
        if (typeid(*this) != typeid(other))
            return false;

        return foo == other.foo;
    }
    
};

#endif