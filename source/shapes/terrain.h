#ifndef TERRAIN_H
#define TERRAIN_H
#include <glm/glm.hpp>
#include "../light/ray.h"
#include "../materials/material.h"
#include "../light/lightsource.h"
#include "shape2D.h"
using glm::vec3;
using glm::vec4;

class Terrain : public Shape2D {
public:
    float** heightmap;
    float height, width;
    vec4 bottom_left;
    vec4 bl;
    vec4 tl;
    Terrain(float** hmap, float h, float w, vec4 bottom_left): heightmap(hmap), height(h), width(w),
    bl(bottom_left), tl() bottom_left(scalevec4(bottom_left)), Shape2D(vec3(0.3, 0.5, 0.3), std::initializer_list<Material*>(), "Terrain"){
    }
    virtual bool intersect(Ray& ray, glm::vec3 dir, glm::vec4& intersectionpoint, glm::vec2* uv = nullptr) 
    {
        const float delt = 0.01f;
        const float mint = 0.001f;
        const float maxt = 10.0f;
        for( float t = mint; t < maxt; t += delt )
        {
            vec4 p = ray.position + ray.direction*t;
            vec4 pt = unscalevec4(p);

            float u =  (((pt.x) - bl.x));
            float v =  (((pt.z) - bl.z));
            if(pt.x < bl.x + width && pt.x > bl.x && pt.z < height + bl.z && pt.z > bl.z){
                if(u > 0 && v > 0 && (int)u < 512 && (int)v < 512 && pt.y > 0 && pt.y < 50 * (heightmap[(int) u ][(int) v  ]) )
                { 
                    intersectionpoint = abs(p.y*vec4(0,-50,0,0)) - p.x*vec4(width,0,0,0) - p.z*vec4(0,0,height,0) + vec4(0,0,0,1);
                    return true;
                }
            }
        }
        return false;
    }

    virtual  glm::vec3 getcolor(Intersection& intersection, const Ray& primary_ray, const std::vector<Shape2D*>& shapes, LightSource* lightSource)   {
        vec3 directLight   = lightSource->getDirectLight(intersection, shapes);
        vec3 indirectLight = lightSource->getIndirectLight();
        vec3 l = (vec3) intersection.direction;
        vec3 norm = intersection.shape2D->getnormal(intersection);
        float prop = dot(normalize(norm), -l);
        float projection_factor = std::max(prop, 0.0f);
        return intersection.shape2D->color * projection_factor * (directLight+indirectLight);
    }

    virtual  vec3 getnormal(Intersection& intersection){
        vec4 pt =  (intersection.position);
        float u =  abs(((pt.x) - bl.x));
        float v =  abs(((pt.z) - bl.z));
        if(u >= 512 || v >= 512)
            return vec3(0,-1,0);
        // printf("%f %f \n", u, v);
        float down =    unscalefloat(heightmap[(int)u]                                  [max(0, min( (int) height - 1, (int)v-4))]  );
        float up =      unscalefloat(heightmap[(int)u]                                  [max(0, min( (int) height - 1, (int)v+4))]  );
        float left =    unscalefloat(heightmap[max(0, min( (int) width - 1, (int)u-4))] [(int)v]                                    );
        float right =   unscalefloat(heightmap[max(0, min( (int) width - 1, (int)u+4))] [(int)v]                                    );
        vec3 yz = vec3(intersection.position.x, up, intersection.position.z + 4.f) - vec3(intersection.position.x, down, intersection.position.z - 4.f);
        vec3 yx = vec3(intersection.position.x + 4.f, left, intersection.position.z) - vec3(intersection.position.x - 4.f, right, intersection.position.z);
        return glm::normalize(cross(yx, yz));
        
    }
    virtual glm::vec4 toworldcoordinates(glm::vec4 cam_intersect){
        
    };

    virtual vec2 getUV(Intersection& intersectpoint){
     
    }
private:
      virtual bool isEqual(const Shape2D& other) const {
          return true;
      }
};

#endif 