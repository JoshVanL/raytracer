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
    vec4 br;
    vec4 tl;
    vec4 tr;
    Terrain(float** hmap, float h, float w, vec4 bl, vec4 br, vec4 tl, vec4 tr, vec3 color): heightmap(hmap), height(h), width(w),
    bl(scalevec4(bl)), br(scalevec4(br)), tl(scalevec4(tl)), tr(scalevec4(tr)), Shape2D(color, std::initializer_list<Material*>(), "Terrain"){
        printf("%f %f %f\n", scalevec4(bl).x, scalevec4(bl).y, scalevec4(bl).z);
    }
    virtual bool intersect(Ray& ray, glm::vec3 dir, glm::vec4& intersectionpoint, glm::vec2* uv = nullptr) 
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
                // printf("true\n");
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
        int specular_exponent = 100;
        float Kd = 0.8; // diffuse weight 
        float Ks = 0.3; // specular weight 
        vec3 directLight   = lightSource->getDirectLight(intersection, shapes);
        vec3 indirectLight = lightSource->getIndirectLight();
        vec3 l = (vec3) intersection.direction;
        vec3 norm = intersection.shape2D->getnormal(intersection);
        float prop = dot(normalize(norm), l);
        float projection_factor = std::max(prop, 0.0f);

        vec3 reflected_dir = normalize(2.0f * dot(l, norm) * norm - l);

        //Viewing direction
        vec3 v = normalize(-vec3(l));

        //Calculate component of viewing direction in the direction of reflected ray
        float specular_highlight = dot(v, reflected_dir);
        float new_specular_highlight = glm::pow(specular_highlight, specular_exponent);

        /* Calculating Specular Component */
        vec3 specular_component = new_specular_highlight * directLight * vec3(1,1,1);
        vec3 diffuse_component = intersection.shape2D->color * projection_factor * (directLight+indirectLight) * 3.5f;

        return specular_component * Ks +  diffuse_component * Kd ;

        
    }

    virtual  vec3 getnormal(Intersection& intersection){
        vec4 p = intersection.position;
        float u = width*((p.x - br.x) / (bl.x - br.x) );
        float v = height*((p.z - br.z) / (tl.z - br.z) );
        
        // printf("%f %f \n", u, v);
        float down  =       unscalefloat(heightmap[(int)u]                                  [max(0, min( (int) height - 1, (int)v-1))]  );
        float up    =       unscalefloat(heightmap[(int)u]                                  [max(0, min( (int) height - 1, (int)v+1))]  );
        float left  =       unscalefloat(heightmap[max(0, min( (int) width - 1, (int)u-1))] [(int)v]                                    );
        float right =       unscalefloat(heightmap[max(0, min( (int) width - 1, (int)u+1))] [(int)v]                                    );

        // float TL    =       unscalefloat(heightmap[max(0, min( (int) width - 1, (int)u-1))][max(0, min( (int) height - 1, (int)v+1))]  );
        // float TR    =       unscalefloat(heightmap[max(0, min( (int) width - 1, (int)u+1))][max(0, min( (int) height - 1, (int)v+1))]  );
        // float BL    =       unscalefloat(heightmap[max(0, min( (int) width - 1, (int)u-1))][max(0, min( (int) height - 1, (int)v-1))]  );
        // float BR    =       unscalefloat(heightmap[max(0, min( (int) width - 1, (int)u+1))][max(0, min( (int) height - 1, (int)v-1))]  );

        vec3 yz = vec3(intersection.position.x, up, intersection.position.z + 1.f) - vec3(intersection.position.x, down, intersection.position.z - 1.f);
        vec3 yx = vec3(intersection.position.x + 1.f, left, intersection.position.z) - vec3(intersection.position.x - 1.f, right, intersection.position.z);

        // vec3 diag1 = vec3(intersection.position.x - 1.f, TL, intersection.position.z + 1.f) - vec3(intersection.position.x + 1.f, BR, intersection.position.z - 1.f);
        // vec3 diag2 = vec3(intersection.position.x + 1.f, TR, intersection.position.z + 1.f) - vec3(intersection.position.x - 1.f, BL, intersection.position.z - 1.f);

                   
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
};

#endif 