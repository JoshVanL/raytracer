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
    float** colormap;
    float height, width, scaled_width, center_x;
    bool isOcean;
    vec3 secondary_color[2] = {vec3(0.737f, 0.929f, 0.1f) , vec3(0.231f, 0.518f, 0.443f)};
    vec4 bottom_left;
    vec4 bl;
    vec4 br;
    vec4 tl;
    vec4 tr;
    Terrain(float** hmap, 
            float h, 
            float w, 
            vec4 bl, 
            vec4 br, 
            vec4 tl, 
            vec4 tr, 
            vec3 color, 
            const bool isOcean = false) : 
            heightmap(hmap), height(h), width(w), isOcean(isOcean), 
            bl(scalevec4(bl)), br(scalevec4(br)), tl(scalevec4(tl)), tr(scalevec4(tr)), 
            scaled_width(scalevec4(bl).x - scalevec4(br).x), 
            center_x(scalevec4(br).x + (scalevec4(bl).x - scalevec4(br).x)/2.f),
            Shape2D(color, std::initializer_list<Material*>(), "Terrain")          
    {
       
    }
    virtual bool intersect( Ray& ray, glm::vec3 dir, glm::vec4& intersectionpoint, glm::vec2* uv = nullptr) 
    {
        const float delt = 0.01f;
        const float mint = 0.001f;
        const float maxt = 10.0f;
        for( float t = mint; t < maxt; t += delt )
        {
            vec4 p = ray.position + ray.direction*t;
     
            float u = (p.x - br.x) / (bl.x - br.x) ;
            float v = (p.z - br.z) / (tl.z - br.z) ;
          
            if(p.x < bl.x  && p.x > br.x && p.z < tl.z  && p.z > bl.z){

                if(p.y < 0 + 0.3f && p.y > -0.15f * (heightmap[(int) (u * width) ][(int) (v * height)  ])+ 0.3f  )
                { 
                    intersectionpoint = p;
                    return true;
                }
            }
        }
        return false;
    }

    virtual  glm::vec3 getcolor(Intersection& intersection, const Ray& primary_ray, const std::vector<Shape2D*>& shapes, LightSource* lightSource)   {
        /**/
        vec4 p = intersection.position;
        float u_ = width - width*((p.x - br.x) / (bl.x - br.x) );
        float v_ = height - height*((p.z - br.z) / (tl.z - br.z) );
        vec3 directLight   = lightSource->getDirectLight(intersection, shapes);
        vec3 indirectLight = lightSource->getIndirectLight();
        vec3 l = (vec3) intersection.direction;
        vec3 norm = intersection.shape2D->getnormal(intersection);
        float projection_factor = std::max(dot(normalize(norm), l), 0.05f);
        /**/
        if(isOcean){
            vec3 col = glm::mix(vec3(0.913f, 0.439f, 0.145f), intersection.shape2D->color, 40.f * pow(abs(p.x - center_x)/scaled_width,3.f) + 0.35f);
            return col * heightmap[(int)((int)u_ ) % (int)width][(int)((int)v_ ) % (int)height] * projection_factor * (2.5f - (v_ * 2.f)/height);
        }
        /**/
        float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        vec3 colormap_color = glm::mix(intersection.shape2D->color, secondary_color[r > 0.5],  heightmap[modu(u_ * 12, width) ][modu(v_ * 12,height) ]);
        return colormap_color * projection_factor;
    }

    virtual  vec3 getnormal(Intersection& intersection){
        vec4 p = intersection.position;
        float u = ((p.x - br.x) / (bl.x - br.x) )*width;
        float v = ((p.z - br.z) / (tl.z - br.z) )*height;
        float down  =       unscalefloat(heightmap[(int)u]                                  [max(0, min( (int) height - 1, (int)v-1))]  );
        float up    =       unscalefloat(heightmap[(int)u]                                  [max(0, min( (int) height - 1, (int)v+1))]  );
        float left  =       unscalefloat(heightmap[max(0, min( (int) width - 1, (int)u-1))] [(int)v]                                    );
        float right =       unscalefloat(heightmap[max(0, min( (int) width - 1, (int)u+1))] [(int)v]                                    );

        vec3 yz = vec3(intersection.position.x, up, intersection.position.z + 1.f) - vec3(intersection.position.x, down, intersection.position.z - 1.f);
        vec3 yx = vec3(intersection.position.x + 1.f, left, intersection.position.z) - vec3(intersection.position.x - 1.f, right, intersection.position.z);
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