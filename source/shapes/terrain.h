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
    Terrain(float** hmap, float h, float w, vec4 bl, vec4 br, vec4 tl, vec4 tr): heightmap(hmap), height(h), width(w),
    bl(scalevec4(bl)), br(scalevec4(br)), tl(scalevec4(tl)), tr(scalevec4(tr)), Shape2D(vec3(0.3, 0.5, 0.3), std::initializer_list<Material*>(), "Terrain"){
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
                if(p.y > 0 && p.y < 1 * (heightmap[(int) (u * width) ][(int) (v * height)  ]) )
                { 
                    // printf("true");
                    vec4 v0 = bl;
                    vec4 v1 = br;
                    vec4 v2 = tl;
                    vec3 e1 = vec3(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
                    vec3 e2 = vec3(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);
                    vec3 b = vec3(p.x - v0.x, p.y - v0.y, p.z - v0.z);
                    mat3 A( -(vec3)dir, e1, e2 );
                    vec3 x = glm::inverse( A ) * b;
                    vec2 u_v(x.y,x.z);

                    if(u_v.x < 0 || u_v.x > 1 || u_v.y > 1 || u_v.y < 0){
                         v0 = bl;
                         v1 = br;
                         v2 = tr;
                         e1 = vec3(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
                         e2 = vec3(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);
                         b = vec3(p.x - v0.x, p.y - v0.y, p.z - v0.z);
                         A = mat3( -(vec3)dir, e1, e2 );
                         x = glm::inverse( A ) * b;
                         u_v = vec2(x.y,x.z);
                    }

                    if(u_v.x < 0 || u_v.x > 1 || u_v.y > 1 || u_v.y < 0)
                        continue;
                    float u_ = u_v[0], v_ = u_v[1];
                    vec4 e1_ = v1 - v0;
                    vec4 e2_ = v2 - v0;
                    vec4 m = v0 + u_ * e1_ + v_ * e2_;
                    m.y = p.y;
                    intersectionpoint = m;
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
        return intersection.shape2D->color /** projection_factor*/ * (directLight+indirectLight);
    }

    virtual  vec3 getnormal(Intersection& intersection){
        return vec3(0,-1,0);
        // vec4 pt =  (intersection.position);
        // float u =  abs(((pt.x) - bl.x));
        // float v =  abs(((pt.z) - bl.z));
        // if(u >= width || v >= height)
        // // printf("%f %f \n", u, v);
        // float down =    unscalefloat(heightmap[(int)u]                                  [max(0, min( (int) height - 1, (int)v-4))]  );
        // float up =      unscalefloat(heightmap[(int)u]                                  [max(0, min( (int) height - 1, (int)v+4))]  );
        // float left =    unscalefloat(heightmap[max(0, min( (int) width - 1, (int)u-4))] [(int)v]                                    );
        // float right =   unscalefloat(heightmap[max(0, min( (int) width - 1, (int)u+4))] [(int)v]                                    );
        // vec3 yz = vec3(intersection.position.x, up, intersection.position.z + 4.f) - vec3(intersection.position.x, down, intersection.position.z - 4.f);
        // vec3 yx = vec3(intersection.position.x + 4.f, left, intersection.position.z) - vec3(intersection.position.x - 4.f, right, intersection.position.z);
        // return glm::normalize(cross(yx, yz));
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