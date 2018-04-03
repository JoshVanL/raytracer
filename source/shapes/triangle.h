#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <glm/glm.hpp>
#include "shape2D.h"
#include <vector>
#include <initializer_list>
#include "../materials/material.h"
using glm::vec3;
using glm::vec4;
using glm::mat3;
using namespace std;
class Triangle : public Shape2D
{
public:
    Shape3D* shape3D = nullptr;
    glm::vec4 v0;
    glm::vec4 v1;
    glm::vec4 v2;
    glm::vec4 normal;
    Material* mat = nullptr;

    bool hasMaterial = false;

    Triangle(glm::vec4 v0, glm::vec4 v1, glm::vec4 v2,
             glm::vec3 color)
        :   Shape2D(color),
            v0(scalevec4(v0)), v1(scalevec4(v1)), v2(scalevec4(v2)), normal(vec4())
    {
    }

    virtual void SetMaterial(Material* material) {
        mat = material;
        hasMaterial = true;
    }

    virtual glm::vec3 getcolor(const int posx, const int posy) override{
        if (hasMaterial) {
            return mat->material_color(posx, posy);
            //return vec3(1, 1, 1);
        }


        return color;
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

    virtual vec3 minPosition() override {
        vec3 minPos = (vec3)v0;
        for (int i = 0; i < 3; i++) {
            if ( v1[i] < minPos[i] ) minPos[i] = v1[i];
        }
        for (int i = 0; i < 3; i++) {
            if ( v2[i] < minPos[i] ) minPos[i] = v2[i];
        }
        return minPos;
    }

    virtual vec3 maxPosition() override {
        vec3 maxPos = (vec3)v0;
        for (int i = 0; i < 3; i++) {
            if ( v1[i] > maxPos[i] ) maxPos[i] = v1[i];
        }
        for (int i = 0; i < 3; i++) {
            if ( v2[i] > maxPos[i] ) maxPos[i] = v2[i];
        }
        return maxPos;
    }

    virtual vec3 midpoint() override {
        vec3 point = vec3(0, 0, 0);
        point.x = (v0.x + v1.x + v2.x) / 3.0;
        point.y = (v0.y + v1.y + v2.y) / 3.0;
        point.z = (v0.z + v1.z + v2.z) / 3.0;
        return point;
    }

    vector<vec4> verticies() override {
        vector<vec4> verts(3);
        verts[0] = v0;
        verts[1] = v1;
        verts[2] = v2;

        return verts;
    }

    vec3 ComputeNormal() {
        vec3 norm;
        glm::vec3 e1 = glm::vec3(v1.x-v0.x,v1.y-v0.y,v1.z-v0.z);
        glm::vec3 e2 = glm::vec3(v2.x-v0.x,v2.y-v0.y,v2.z-v0.z);
        glm::vec3 normal3 = glm::normalize( glm::cross( e2, e1 ) );
        norm.x = normal3.x;
        norm.y = normal3.y;
        norm.z = normal3.z;
        return norm;
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

};
#endif
