#ifndef SHAPE2D_H
#define SHAPE2D_H
#include <typeinfo>
#include <glm/glm.hpp>
#include <glm/gtx/normal.hpp>


class Ray;

using glm::vec3;
using glm::vec4;
class Shape2D{
public:
    const float L = 555;// Length of Cornell Box side.

    vec3 color, emissionColor, gloss;
    float transparency, reflection; 
    std::string name;
    Shape2D(vec3 color, vec3 gloss, vec3 emissCol, float transp, float refl): 
        color(color), gloss(gloss), emissionColor(emissCol), transparency(transp), reflection(refl){

    };

    Shape2D(vec3 color) : color(color), gloss(vec3(0,0,0)), emissionColor(vec3(0,0,0)), transparency(0), reflection(0){

    }

    Shape2D(vec3 color, vec3 gloss) : color(color), gloss(gloss), emissionColor(vec3(0,0,0)), transparency(0), reflection(0){

    }
    vec4 scalevec4(vec4 v) {
        v *= 2/L;
        v -= vec4(1,1,1,1);
        v.x *= -1;
        v.y *= -1;
        v.w = 1.0;
        return v;
    }
    vec3 scalevec3(vec3 v) {
        v *= 2/L;
        v -= vec3(1,1,1);
        v.x *= -1;
        v.y *= -1;
        return v;
    }
    float scalefloat(float v) {
        v *= 2/L;
        return v;
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