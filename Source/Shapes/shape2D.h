#ifndef SHAPE2D_H
#define SHAPE2D_H
#include <typeinfo>
#include <glm/glm.hpp>
#include <glm/gtx/normal.hpp>


class Ray;

using glm::vec3;

class Shape2D{
public:
    int foo = 0;
    const float L = 555;			// Length of Cornell Box side.

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
    virtual glm::vec3 getnormal(glm::vec4 start, glm::vec4 dir) = 0;
    bool operator==(const Shape2D& other) const
    {
        // If the derived types are the same then compare them
        return typeid(*this) == typeid(other) && isEqual(other);
    }
private:
    virtual bool isEqual(const Shape2D& other) const = 0;
};

#endif