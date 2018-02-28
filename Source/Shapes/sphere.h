#ifndef SPHERE_H
#define SPHERE_H

#include "shape2D.h"
#include "../Light/intersection.h"
#include "../Light/ray.h"
#include "../Materials/solid.h"
#include "../Materials/gloss.h"
#include <vector>
#include <initializer_list>
#define MAX_RAY_DEPTH_ 5
#define M_PI_ 3.141592653589793 

using glm::vec3;
using glm::vec4;
using glm::normalize;
using glm::dot;

using namespace std;
// template<Material* material, ... >;
class Sphere : public Shape2D {
public:
    glm::vec4 center;                         
    float radius, radius2;                     

    Sphere(
        glm::vec4 &cent, 
        float r,
        glm::vec3 col,
        vec3 gloss=vec3(1,1,1),
        const std::initializer_list<Material*>& materials = std::initializer_list<Material*>()
        ) : radius(scalefloat(r)), radius2(pow(scalefloat(r), 2)), center(scalevec4(cent)), Shape2D(col, gloss, materials)
    {   
        name = "Sphere";
    };

    virtual bool intersect(Ray &ray, vec3 dir, vec4& intersection) override
    {
        vec3 rayorig = (vec3) ray.position;
        dir = normalize(vec3(ray.direction));

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
    
    virtual glm::vec3 getcolor(Intersection& intersection, const Ray& ray, const std::vector<Shape2D*>& shapes, LightSource* lightSource)  override {
        
        vector<vec3> colors;
        for(int a = 0; a < materials.size(); a++){
            colors.push_back(materials[a]->material_color(intersection, ray, shapes, lightSource));
        }
        if(colors.size() == 0)
            return vec3(0,0,0);
        if(colors.size() == 1)
            return colors[0];

        vec3 t_color = glm::mix(colors[0], colors[1], 0.5f);    
        for(int a = 2; a < colors.size(); a++){
            t_color = glm::mix(t_color, colors[a], 0.5f);
        }
        return t_color;
    }
    virtual glm::vec3 getnormal(Intersection& intersection)   {
        return (vec3) normalize(intersection.position - center);
    }
    virtual glm::vec4 toworldcoordinates(vec4 cam_intersect) override{
        return cam_intersect;
    }

    virtual vec3 minPosition() override {
        return vec3{center.x - radius, center.y - radius, center.z - radius};
    }
    virtual vec3 maxPosition() override {
        return vec3{center.x + radius, center.y + radius, center.z + radius};
    }
    virtual vec3 midpoint() override {
        return vec3{center.x, center.y, center.z};
    }


private:
    virtual bool isEqual(const Shape2D& other) const override {
        Sphere othersphere = static_cast<const Sphere&>(other);
        if(center == othersphere.center && radius == othersphere.radius)
            return true;
        return false;
    }
    // bool intersect(vec4 start, vec4 t_dir, vec4& intersection)
    // {
    //     vec3 rayorig = vec3(start);
    //     vec3 dir = normalize(vec3(t_dir));

    //     vec3 L = (vec3) center - rayorig;
    //     float tca = dot(L, dir);
    //     float d2 = dot(L, L) - tca * tca;
    //     if (d2 > (radius2 + radius2*0.1f)) { 
    //         return false;
    //     }

    //     float thc = sqrt(radius2 - d2);
    //     float t0 = tca - thc;
    //     float t1 = tca + thc;

    //     if (t0 > t1) {
    //         std::swap(t0, t1); 
    //     }

    //     if (t0 < 0) {
    //         t0 = t1; 
    //         if (t0 < 0) {
    //             return false; 
    //         }
    //     }
    //     float t = t0;

    //     vec3 temp = vec3(start) + (t * dir);
        
    //     intersection = vec4(temp.x, temp.y, temp.z, 1);
    //     return true;
    // }

};
#endif
