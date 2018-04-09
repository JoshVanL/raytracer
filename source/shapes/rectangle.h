#ifndef RECTANGLE_H
#define RECTANGLE_H
#include <cmath>
#include <glm/glm.hpp>
#include "shape2D.h"
#include "../light/intersection.h"
#include "../light/ray.h"
#include <initializer_list>
#include "../materials/material_properties/gloss.h"
#include "../materials/material.h"
#include "../materials/diffuse.h"
#include <iostream>
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat3; 

class Rectangle : public Shape2D
{
public:
    float area;
    glm::vec4 v0;
    glm::vec4 v1;
    glm::vec4 v2;
    glm::vec4 v3;
    glm::vec4 normal;
    std::vector<std::vector<vec3>> v;
    Rectangle(glm::vec4 v0, 
              glm::vec4 v1, 
              glm::vec4 v2,
              glm::vec4 v3,  
             glm::vec3 color, 
             std::string id,
             const std::initializer_list<Material*>& materials = std::initializer_list<Material*>())
        :   Shape2D(color, check_mats(materials), id), 
            v0((scalevec4(v0))), v1(scalevec4(v1)), v2(scalevec4(v2)), v3(scalevec4(v3)), normal(ComputeNormal())
    {
    }
    vector<Material*> check_mats(vector<Material*> mats){
        if(mats.empty()){
            vector<Material*> initmaterials;
            Diffuse* dif = new Diffuse();
            initmaterials.push_back(dif);
            return initmaterials;
        }
        return mats;
    }
    Rectangle(glm::vec4 v0, 
             glm::vec4 v1, 
             glm::vec4 v2, 
             glm::vec4 v3, 
             glm::vec3 color,
             std::string id,
             bool scale,
             const std::initializer_list<Material*>& materials = std::initializer_list<Material*>()
             )
        :   Shape2D(color, materials, id), 
            v0(v0), v1(v1), v2(v2), normal(ComputeNormal())
    {
    }

    virtual vec2 getUV(Intersection& intersectpoint) override {

    }
    
    virtual bool intersect(Ray& ray, vec3 dir, vec4& intersectionpoint, glm::vec2* uv = nullptr) override {
        vec3 e1 = vec3(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
        vec3 e2 = vec3(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);
        vec3 b1  = vec3(ray.position.x - v0.x, ray.position.y - v0.y, ray.position.z - v0.z);
       
        vec3 e3 = vec3(v1.x - v3.x, v1.y - v3.y, v1.z - v3.z);
        vec3 e4 = vec3(v2.x - v3.x, v2.y - v3.y, v2.z - v3.z);
        vec3 b2 = vec3(ray.position.x - v3.x, ray.position.y - v3.y, ray.position.z - v3.z);
        mat3 A( -dir, e1, e2 );
        mat3 B( -dir, e3, e4 );
        vec3 x1 = glm::inverse( A ) * b1;
        vec3 x2 = glm::inverse( B ) * b2;

        float t1 = x1.x;
        float u1 = x1.y;
        float v1 = x1.z;

        float t2 = x2.x;
        float u2 = x2.y;
        float v2 = x2.z; 

        if (0 <= t1 && 0 <= u1 && 0 <= v1 && u1 + v1 <= 1) {
            intersectionpoint = toworldcoordinates(vec4(t1,u1,v1,1));
            return true;
        } 
        else if (0 <= t2 && 0 <= u2 && 0 <= v2 && u2 + v2 <= 1) { 
            intersectionpoint = toworldcoordinates2(vec4(t2,u2,v2,1));
            return true;
        } 
        return false;
    }

    virtual glm::vec3 getcolor(Intersection& intersection, const Ray& primary_ray, const std::vector<Shape2D*>& shapes, LightSource* lightSource) override{
        if(materials.empty())   return color;
        vector<vec3> colors;

        vec3 directLight = lightSource->getDirectLight(intersection, shapes);
        vec3 indirectLight = lightSource->getIndirectLight();
        for(int a = 0; a < materials.size(); a++){
            colors.push_back(materials[a]->material_color(intersection, primary_ray, shapes, lightSource, directLight, indirectLight));
        }
        vec3 t_color = vec3(0,0,0);
        for(int a = 0; a < colors.size(); a++){
            if(colors.size() == 1)
               return colors[0] + (color * vec3(0.02, 0.02, 0.02));
            t_color = glm::mix(t_color, colors[a], 0.5f);
        }
        return t_color;
    }

    virtual vec4 toworldcoordinates(glm::vec4 cam_intersect) override {
        float u = cam_intersect[1], v = cam_intersect[2];
        vec4 e1 = v1 - v0;
        vec4 e2 = v2 - v0;
        return v0 + u * e1 + v * e2;
    }
    vec4 toworldcoordinates2(glm::vec4 cam_intersect){
        float u = cam_intersect[1], v = cam_intersect[2];
        vec4 e3 = v1 - v3;
        vec4 e4 = v2 - v3;
        return v3 + u * e3 + v * e4;
    }

    virtual vec3 getnormal(Intersection& intersection){
        if(glm::dot(normal, glm::normalize(intersection.direction)) < 0){
            return -vec3(normal);
        }
        else{
            return vec3(normal);
        }
    }

    virtual vec3 minPosition() override {
        vec3 minPos = (vec3)v0;
        for (int i = 0; i < 3; i++) {
            if ( v1[i] < minPos[i] ) minPos[i] = v1[i];
            if ( v2[i] < minPos[i] ) minPos[i] = v2[i];
        }
        return minPos;
    }

    virtual vec3 maxPosition() override {
        vec3 maxPos = (vec3)v0;
        for (int i = 0; i < 3; i++) {
            if ( v1[i] > maxPos[i] ) maxPos[i] = v1[i];
            if ( v2[i] > maxPos[i] ) maxPos[i] = v2[i];
        }
        return maxPos;
    }

    virtual vec3 midpoint() override {
        vec3 point = vec3(0, 0, 0);
        point.x = (v0.x + v1.x + v2.x+ v3.x) / 4.0;
        point.y = (v0.y + v1.y + v2.y+ v3.y) / 4.0;
        point.z = (v0.z + v1.z + v2.z+ v3.z) / 4.0;
        return point;
    }

private:
    virtual bool isEqual(const Shape2D& other) const override {
        Rectangle tri = static_cast<const Rectangle&>(other);
        if( (v0 == tri.v0 && v1 == tri.v1 && v2 == tri.v2 && v2 == tri.v3)){
            return true;
        }
        return false;
    }

    vec4 ComputeNormal()
    {
        return glm::normalize(vec4(glm::triangleNormal((vec3) v0, (vec3) v1, (vec3) v2), 1));
    }
};
#endif