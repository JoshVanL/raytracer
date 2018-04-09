#ifndef CLOUD_H
#define CLOUD_H
#include <glm/glm.hpp>
#include "../light/ray.h"
#include "../materials/material.h"
#include "../light/lightsource.h"
#include "shape2D.h"
using glm::vec3;
using glm::vec4;

class Cloud : public Shape2D {
public:
    float** heightmap;
    float** colormap;
    float height, width;
    bool isOcean;
    float radius, radius2;
    vec4 bottom_left;
    vec4 center;
    Cloud(float** hmap, 
            float h, 
            float w,
            float radius, 
            vec4 center,
            vec3 color) : 
            heightmap(hmap), height(h), width(w), center(center), radius(scalefloat(radius)), radius2(pow(scalefloat(radius),2)),
            Shape2D(color, std::initializer_list<Material*>(), "Terrain")
    {
       
    }
    virtual bool intersect( Ray& ray, glm::vec3 dir, glm::vec4& intersectionpoint, glm::vec2* uv = nullptr) 
    {
        const float delt = 0.01f;
        const float mint = 0.001f;
        const float maxt = 10.0f;
        float maxdis = scalefloat(radius);

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
        vec4 sphere_intersect = vec4(temp.x, temp.y, temp.z, 1);
intersectionpoint = sphere_intersect; return true;
        // for( float t = mint; t < maxt; t += delt )
        // {
        //     vec4 p = sphere_intersect - ray.direction*t;
     
        //     float u = (p.x - center.x + center.x/2.f) / (radius * 2.f);
        //     float v = (p.y - center.y + center.y/2.f) / (radius * 2.f);

        //     if(p.z <   0.1f  * (heightmap[(int) (u * width) ][(int) (v * height)  ]) + sphere_intersect.z)
        //     { 
        //         intersectionpoint = p;
        //         return true;
        //     }
            
        // }
        // return false;
    }


    virtual  glm::vec3 getcolor(Intersection& intersection, const Ray& primary_ray, const std::vector<Shape2D*>& shapes, LightSource* lightSource)   {
        /**/
        vec4 p = intersection.position;
        // float v_ = height - height*((p.y - br.y) / (tl.y - br.y) );
        vec3 directLight   = lightSource->getDirectLight(intersection, shapes);
        vec3 indirectLight = lightSource->getIndirectLight();
        vec3 l = (vec3) intersection.direction;
        vec3 norm = intersection.shape2D->getnormal(intersection);
        float projection_factor = std::max(dot(normalize(norm), l), 0.1f);
        return intersection.shape2D->color  * projection_factor;// * (3.f - (v_ * 2.f)/height);
    }

    virtual  vec3 getnormal(Intersection& intersection){
        // return vec3(0,0,-1);
        vec4 p = intersection.position;
        float u = (p.x - center.x + center.x/2.f) / (radius * 2.f);
        float v = (p.y - center.y + center.y/2.f) / (radius * 2.f);
        float down  =       unscalefloat(heightmap[(int)u]                                  [max(0, min( (int) height - 1, (int)v-1))]  );
        float up    =       unscalefloat(heightmap[(int)u]                                  [max(0, min( (int) height - 1, (int)v+1))]  );
        float left  =       unscalefloat(heightmap[max(0, min( (int) width - 1, (int)u-1))] [(int)v]                                    );
        float right =       unscalefloat(heightmap[max(0, min( (int) width - 1, (int)u+1))] [(int)v]                                    );

        vec3 yz = vec3(intersection.position.x, intersection.position.y + 1.f, up) - vec3(intersection.position.x, intersection.position.y - 1.f, down);
        vec3 yx = vec3(intersection.position.x + 1.f, intersection.position.y, left) - vec3(intersection.position.x - 1.f,  intersection.position.y, right);
        return  glm::normalize(cross(yx, yz));
    }

    virtual glm::vec4 toworldcoordinates(glm::vec4 cam_intersect){
        
    };

    virtual vec2 getUV(Intersection& intersectpoint){
     
    }
private:
    virtual bool isEqual(const Shape2D& other) const {
        return true;
    }
    int modu(float a, float b){
        if(a < 0){
            float a_p = abs(a);
            while(a < 0){
                a+= a_p;
            }
            return (int)a;
        }
        else{
            return ((int)a % (int)b + (int)a) % (int)b;
        }
    }
};


#endif 