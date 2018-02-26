#include <typeinfo>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/normal.hpp>

using glm::vec3;
using glm::vec4;

#ifndef SHAPE2D_H
#define SHAPE2D_H

class Ray;
class Material;
class Intersection;
class Shape3D;
class LightSource;

class Shape2D{
public:
    const float L = 1000;// Length of Cornell Box side.

    vec3    color;
    vec3    gloss;
    Material* material = nullptr;
    Shape3D* shape3D = nullptr;
    Shape2D(vec3 color, vec3 gloss, Material* material, Shape3D* shape3D = nullptr): 
        color(color), gloss(gloss), material(material){
    };

    Shape2D(vec3 color, vec3 gloss, Shape3D* shape3D = nullptr) : color(color), gloss(gloss), shape3D(shape3D) {

    };

    Shape2D(vec3 color, Shape3D* shape3D = nullptr) : color(color), gloss(vec3(1,1,1)), shape3D(shape3D){

    };
    Shape2D(vec3 color, Material* material, Shape3D* shape3D = nullptr) : material(material), color(color), gloss(vec3(1,1,1)), shape3D(shape3D){

    };

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
    virtual glm::vec3 getcolor(Intersection& intersection, const Ray& primary_ray, const std::vector<Shape2D*>& shapes, LightSource& lightSource) = 0;
    virtual bool intersect(Ray& ray, glm::vec3 dir, glm::vec4& intersectionpoint) = 0;
    virtual glm::vec4 toworldcoordinates(glm::vec4 cam_intersect) = 0;
    virtual glm::vec3 getnormal(glm::vec4 start, glm::vec4 dir) = 0;

    virtual vec3 minPosition() {
        return vec3();
    };
    virtual vec3 maxPosition() {
        return vec3();
    };
    virtual vec3 midpoint() {
        return vec3();
    };

    virtual std::vector<vec4> verticies() {
        return std::vector<vec4>();
    }

    bool operator==(const Shape2D& other) const
    {
        // If the derived types are the same then compare them
        return typeid(*this) == typeid(other) && isEqual(other);
    }
private:
    virtual bool isEqual(const Shape2D& other) const = 0;
};

#endif
