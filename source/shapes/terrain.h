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
    vec4 top_left;
    vec4 bottom_right;
    vec4 bl;
    Terrain(float** hmap, float h, float w, vec4 bottom_left, vec4 bottom_right, vec4 top_left): heightmap(hmap), height(h), width(w),
    bl(bottom_left), bottom_left(scalevec4(bottom_left)), top_left(scalevec4(top_left)), bottom_right(scalevec4(bottom_right))
    , Shape2D(vec3(0.3, 0.5, 0.3), std::initializer_list<Material*>(), "Terrain"){
        // printf("%f %f %f \n", this->bottom_left.x, this->bottom_left.y, this->bottom_left.z);
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

            float u =  abs(((pt.x) - bl.x));
            float v =  abs(((pt.z) - bl.z));
            
            if(pt.x < bl.x + width && pt.x > bl.x && pt.z < height + bl.z && pt.z > bl.z){
                if((int)u < 512 && (int)v < 512 && pt.y > 0 && pt.y < 50 * (heightmap[(int) u ][(int) v  ]) )
                { 
                    float t_u = abs(p.x - bottom_left.x);

                    float t_v = abs(p.z - bottom_left.z);
                    vec4 e1 = top_left - bottom_left;
                    vec4 e2 = bottom_right - bottom_left;
                    intersectionpoint = bottom_left + t_u * e1 + t_v * e2;
                    // intersectionpoint = bottom_left + abs(p.y*vec4(0,50,0,0)) + p.x*vec4(width,0,0,0) + p.z*vec4(0,0,height,0) + vec4(0,0,0,1);
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
        return vec3(0.5,0.5,0.5);
    }

    virtual  vec3 getnormal(Intersection& intersection){
        // return vec3(0,-1,0);
        vec4 pt =  (intersection.position);
        float u =  abs(pt.x * width);
        float v =  abs(pt.z * height);
        printf("%f %f \n", u, v);
        if(u > 511 || v > 511)
            return vec3(0,-1,0);
    
        float down =    unscalefloat(heightmap[(int)u]                                  [max(0, min( (int) height - 1, (int)v-4))]  );
        float up =      unscalefloat(heightmap[(int)u]                                  [max(0, min( (int) height - 1, (int)v+4))]  );
        float left =    unscalefloat(heightmap[max(0, min( (int) width - 1, (int)u-4))] [(int)v]                                    );
        float right =   unscalefloat(heightmap[max(0, min( (int) width - 1, (int)u+4))] [(int)v]                                    );
        vec3 yz = vec3(pt.x, up, pt.z + 4.f) - vec3(pt.x, down, pt.z - 4.f);
        vec3 yx = vec3(pt.x + 4.f, left, pt.z) - vec3(pt.x - 4.f, right, pt.z);
        return glm::normalize(cross(yz, yx));
        
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