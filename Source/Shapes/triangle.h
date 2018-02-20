#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <glm/glm.hpp>
#include "shape2D.h"
#include "../Light/intersection.h"
#include "../Light/ray.h"
using glm::vec3;
using glm::vec4;
using glm::mat3;

class Triangle : public Shape2D
{
public:
    glm::vec4 v0;
    glm::vec4 v1;
    glm::vec4 v2;
    glm::vec4 normal;

    Triangle(glm::vec4 v0, glm::vec4 v1, glm::vec4 v2, 
             glm::vec3 color, glm::vec3 gloss, Material* mat)
        :   Shape2D(color, gloss, mat), 
            v0(scalevec4(v0)), v1(scalevec4(v1)), v2(scalevec4(v2)), normal(ComputeNormal())
    {
         name = "Triangle";
    }

    Triangle(glm::vec4 v0, glm::vec4 v1, glm::vec4 v2, 
             glm::vec3 color, glm::vec3 gloss)
        :   Shape2D(color, gloss), 
            v0(scalevec4(v0)), v1(scalevec4(v1)), v2(scalevec4(v2)), normal(ComputeNormal())
    {
         name = "Triangle";
    }

    virtual bool intersect(Ray& ray, vec3 dir, vec4& intersectionpoint) override {

        vec3 e1 = vec3(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
        vec3 e2 = vec3(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);
        vec3 b = vec3(ray.position.x - v0.x, ray.position.y - v0.y, ray.position.z - v0.z);
        mat3 A( -dir, e1, e2 );
        vec3 x = glm::inverse( A ) * b;

        float t = x.x;
        float u = x.y;
        float v = x.z; 

        if (0 <= t && 0 <= u && 0 <= v && u + v <= 1) { 
            intersectionpoint = toworldcoordinates(vec4(t,u,v,1));
            return true;
        } 
        return false;
    }
    virtual glm::vec3 getcolor(Intersection& intersection, const Ray& primary_ray, const std::vector<Shape2D*>& shapes) override{
        vec3 t_color;
        if(material){
            t_color = material->material_color(intersection, primary_ray, shapes) * gloss;
            return t_color;
        }
        else{
            return color;
        }
    }
    virtual vec4 toworldcoordinates(glm::vec4 cam_intersect) override {
        float u = cam_intersect[1], v = cam_intersect[2];
        vec4 e1 = v1 - v0;
        vec4 e2 = v2 - v0;
        return v0 + u * e1 + v * e2;
    }

    virtual vec3 getnormal(vec4 start, vec4 dir){
        return  glm::normalize(glm::triangleNormal((vec3) v0, (vec3) v1, (vec3) v2));
    }


private:
    virtual bool isEqual(const Shape2D& other) const override {
        Triangle tri = static_cast<const Triangle&>(other);
        if( (v0 == tri.v0 && v1 == tri.v1 && v2 == tri.v2) ||
            (v0 == tri.v0 && v1 == tri.v2 && v2 == tri.v1) ||
            (v0 == tri.v1 && v1 == tri.v2 && v2 == tri.v0) ||
            (v0 == tri.v1 && v1 == tri.v0 && v2 == tri.v2) ||
            (v0 == tri.v2 && v1 == tri.v0 && v2 == tri.v1) ||
            (v0 == tri.v2 && v1 == tri.v1 && v2 == tri.v0)){
            return true;
        }
        return false;
    }

    vec4 ComputeNormal()
    {
        vec4 norm;
        glm::vec3 e1 = glm::vec3(v1.x-v0.x,v1.y-v0.y,v1.z-v0.z);
        glm::vec3 e2 = glm::vec3(v2.x-v0.x,v2.y-v0.y,v2.z-v0.z);
        glm::vec3 normal3 = glm::normalize( glm::cross( e2, e1 ) );
        norm.x = normal3.x;
        norm.y = normal3.y;
        norm.z = normal3.z;
        norm.w = 1.0;
        return norm;
    }
};
#endif