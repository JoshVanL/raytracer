#ifndef SPHERE_H
#define SPHERE_H
#include "SDL.h"
#include "shape2D.h"
#include "../Light/intersection.h"
#include "../Light/ray.h"
#include <vector>

#define MAX_RAY_DEPTH_ 5
#define M_PI_ 3.141592653589793 

using glm::vec3;
using glm::vec4;
using glm::normalize;
using glm::dot;

using namespace std;

class Sphere : public Shape2D {
public:
    glm::vec4 center;                         
    float radius, radius2;                     

    Sphere(
        glm::vec4 &cent, float r,
        glm::vec3 col,
        glm::vec3 gloss,
        Material* material) : radius(scalefloat(r)), radius2(pow(scalefloat(r), 2)), center(scalevec4(cent)), Shape2D(col, gloss, material)
    {   
        name = "Sphere";
    };
    Sphere(glm::vec4 &cent, float &r, glm::vec3 col) : 
        center(scalevec4(cent)), radius(scalefloat(r)), radius2(pow(scalefloat(r), 2)), 
        Shape2D(col) 
    {
        name = "Sphere";
    };

    virtual bool intersect(Ray &ray, vec3 dir, vec4& intersection) override
    {
        vec3 rayorig = vec3(ray.position);
        dir = normalize(vec3(dir));

        vec3 L = (vec3) center - rayorig;
        float tca = dot(L, dir);
        float d2 = dot(L, L) - tca * tca;
        if (d2 > radius2) { 
            return false;
        }

        float thc = sqrt(radius2 - d2);
        float t0 = tca - thc;
        float t1 = tca + thc;

        if (t0 > t1) {
            std::swap(t0, t1); 
        }
        if (t0 < 0) {
            t0 = t1; 
            if (t0 < 0) {
                return false;
            }
        }
        float t = t0;
        vec3 temp = vec3(ray.position) + (t * dir);
        intersection = vec4(temp.x, temp.y, temp.z, 1);
        return true;
    }
    virtual glm::vec3 compute_color() override {
        vec3 t_color;
        if(material)
            return material->material_color(this);
        else 
            return color;
    }
    virtual glm::vec3 getnormal(vec4 start, vec4 dir) override {
        glm::vec4 intersectpoint;
        if(intersect(start, dir, intersectpoint)){
            vec3 intersectDir = (vec3) intersectpoint - (vec3) start;
            vec3 intersectNormal = intersectDir - (vec3) center; 
            return glm::normalize(intersectNormal);
        }
    }
    virtual glm::vec4 toworldcoordinates(vec4 cam_intersect) override{
        return cam_intersect;
    }
private:
    virtual bool isEqual(const Shape2D& other) const override {
        Sphere othersphere = static_cast<const Sphere&>(other);
        if(center == othersphere.center && radius == othersphere.radius)
            return true;
        return false;
    }
    bool intersect(vec4 start, vec4 t_dir, vec4& intersection)
    {
        vec3 rayorig = vec3(start);
        vec3 dir = normalize(vec3(t_dir));

        vec3 L = (vec3) center - rayorig;
        float tca = dot(L, dir);
        float d2 = dot(L, L) - tca * tca;
        if (d2 > (radius2 + radius2*0.1f)) { 
            return false;
        }

        float thc = sqrt(radius2 - d2);
        float t0 = tca - thc;
        float t1 = tca + thc;

        if (t0 > t1) {
            std::swap(t0, t1); 
        }

        if (t0 < 0) {
            t0 = t1; 
            if (t0 < 0) {
                return false; 
            }
        }
        float t = t0;

        vec3 temp = vec3(start) + (t * dir);
        
        intersection = vec4(temp.x, temp.y, temp.z, 1);
        return true;
    }

};
#endif