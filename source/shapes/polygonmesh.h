#ifndef POLYGONMESH_H
#define POLYGONMESH_H 

#include "triangle.h"
#include <vector>
#include <glm/glm.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
//////////////////////////////////////// 
// UNUSED
////////////////////////////////////////
using glm::vec4;
using glm::vec3;
using glm::vec2;

#define INFINITY_ 1e8 
class PolygonMesh {
public:
    // member variables
    uint32_t numTris;                         // number of triangles 
    Triangle** triangles;
    vec3* triVertexPositions;              // triangles vertex position 
    uint32_t* triIndexes;         // vertex index array 
    vec3* triNormals;                        // triangles vertex normals 
    vec2* triTexCoordinates;       // triangles texture coordinates 
    vec3 color = vec3(0.3, 0.5, 0.75);
    PolygonMesh(uint nfaces, 
                uint faceIndexes[], 
                uint vertexIndexes[], 
                vec3 vertices[], 
                vec3 normals[],
                vec2 st[]):numTris(0)
    {



        int k = 0, maxVertexIndex = 0;
        for (uint32_t i = 0; i < nfaces; ++i) { 
            numTris += faceIndexes[i] - 2; 
            for (uint32_t j = 0; j < faceIndexes[i]; ++j) 
                if (vertexIndexes[k + j] > maxVertexIndex) 
                    maxVertexIndex = vertexIndexes[k + j]; 
            k += faceIndexes[i]; 
        } 
        maxVertexIndex += 1; 

        ///
        triangles = (Triangle**) malloc(sizeof(Triangle*)*numTris );
        triVertexPositions = new vec3[maxVertexIndex];
        for(int i = 0; i < maxVertexIndex; i++){
            triVertexPositions[i] = vertices[i];
        }
        for(int i = 0; i < numTris; i ++){
            Triangle* tri = new Triangle(vec4(triVertexPositions[i*3], 1.f), vec4(triVertexPositions[i*3+1], 1.f), vec4(triVertexPositions[i*3+2], 1.f), 
                            vec3(((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) ,
                                 (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) ,
                                 (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) ))), "tehe", false, {new Diffuse()});
            triangles[i] = tri;
        }
        ///
        triIndexes = new uint32_t[3*numTris];
        triNormals  = new vec3[3*numTris];
        triTexCoordinates = new vec2[3*numTris];
        uint32_t l = 0;
        for (uint32_t i = 0, k = 0; i < nfaces; ++i) { // for each  face 
            for (uint32_t j = 0; j < faceIndexes[i] - 2; ++j) { // for each triangle in the face 
                triIndexes[l] = vertexIndexes[k]; 
                triIndexes[l + 1] = vertexIndexes[k + j + 1]; 
                triIndexes[l + 2] = vertexIndexes[k + j + 2]; 
                triNormals[l] = normals[k]; 
                triNormals[l + 1] = normals[k + j + 1]; 
                triNormals[l + 2] = normals[k + j + 2]; 
                triTexCoordinates[l] = st[k]; 
                triTexCoordinates[l + 1] = st[k + j + 1]; 
                triTexCoordinates[l + 2] = st[k + j + 2]; 
                l += 3; 
            } 
            k += faceIndexes[i]; 
        } 
        for(int i = 0; i < nfaces; i++){
            std::cout << triVertexPositions[i].x << " " << triVertexPositions[i].x << " " << triVertexPositions[i].x << "\n";
            
        }
        std::cout << "numtris " << numTris << "\n";
    };

    bool rayTriIntersect(const vec3 orig, const vec3 dir, const vec3 v0, const vec3 v1, const vec3 v2,  float &t, float& u, float& v) const{
        vec3 v0v1 = v1 - v0; 
        vec3 v0v2 = v2 - v0; 
        vec3 pvec = cross(dir,(v0v2)); 
        float det = dot(v0v1,pvec); 
    
        // ray and triangle are parallel if det is close to 0
        if (fabs(det) < INFINITY_) return false; 
    
        float invDet = 1 / det; 
    
        vec3 tvec = orig - v0; 
        u = dot(tvec,(pvec)) * invDet; 
        if (u < 0 || u > 1) return false; 
    
        vec3 qvec = cross(tvec, v0v1); 
        v = dot(dir,qvec) * invDet; 
        if (v < 0 || u + v > 1) return false; 
    
        t = dot(v0v2,qvec) * invDet; 
    
        return true; 
    }
    

    bool intersect(const vec3 &orig, const vec3 &dir, float &closestDist, uint &triIndex, vec2 &uv) const 
    {
       
        // uint j = 0; 
        // bool isect = false; 
        // for (uint i = 0; i < numTris; ++i) { 
        //     const vec3 &v0 = triVertexPositions[triIndexes[j]]; 
        //     const vec3 &v1 = triVertexPositions[triIndexes[j + 1]]; 
        //     const vec3 &v2 = triVertexPositions[triIndexes[j + 2]]; 
        //     float t = INFINITY_, u, v; 
        //     if (rayTriIntersect(orig, dir, v0, v1, v2, t, u, v) && t < closestDist) { 
        //         closestDist = t; 
        //         uv.x = u; 
        //         uv.y = v; 
        //         triIndex = i; 
        //         isect = true; 
        //     } 
        //     j += 3; 
        // } 

        // return isect; 
    }

    vec3 getcolor(
        /*const uint32_t &triIndex, 
        const vec2 &uv, 
        vec3 &hitNormal*/)
    {
        return color;
    }

    void getNormal(
        const uint32_t &triIndex, 
        const vec2 &uv, 
        vec3 &hitNormal) 
    { 
        // face normal
        const vec3 &v0 = triVertexPositions[triIndexes[triIndex * 3]]; 
        const vec3 &v1 = triVertexPositions[triIndexes[triIndex * 3 + 1]]; 
        const vec3 &v2 = triVertexPositions[triIndexes[triIndex * 3 + 2]]; 
        hitNormal = glm::normalize(glm::cross((v1 - v0),(v2 - v0))); 
    } 

    void getTextureCoordinates( 
        const uint32_t &triIndex, 
        const vec2 &uv, 
        vec2 &hitTextureCoordinates){
         
        // texture coordinates
        const vec2 &st0 = triTexCoordinates[triIndex * 3]; 
        const vec2 &st1 = triTexCoordinates[triIndex * 3 + 1]; 
        const vec2 &st2 = triTexCoordinates[triIndex * 3 + 2]; 
        hitTextureCoordinates = (1 - uv.x - uv.y) * st0 + uv.x * st1 + uv.y * st2; 
 
    }

    static PolygonMesh* generatePolySphere(float rad, uint32_t divs) 
    { 
        // generate points                                                                                                                                                                                      
        uint32_t numVertices = (divs - 1) * divs + 2; 
        vec3* P = new vec3[numVertices]; 
        vec3* N = new vec3[numVertices]; 
        vec2* st =new vec2[numVertices]; 
    
        float u = -M_PI_2; 
        float v = -M_PI; 
        float du = M_PI / divs; 
        float dv = 2 * M_PI / divs; 
    
        P[0] = N[0] = vec3(0, -rad, 0); 
        uint32_t k = 1; 
        for (uint32_t i = 0; i < divs - 1; i++) { 
            u += du; 
            v = -M_PI; 
            for (uint32_t j = 0; j < divs; j++) { 
                float x = rad * cos(u) * cos(v) * 15; 
                float y = rad * sin(u)  * 15; 
                float z = rad * cos(u) * sin(v) * 15 ; 
                P[k] = N[k] = vec3(x, y, z); 
                st[k].x = u / M_PI + 0.5; 
                st[k].y = v * 0.5 / M_PI + 0.5; 
                v += dv, k++; 
            } 
        } 
        P[k] = N[k] = vec3(0, rad, 0); 
    
        uint32_t npolys = divs * divs; 
        uint32_t* faceIndex = new uint32_t[npolys]; 
        uint32_t* vertsIndex = new uint32_t[(6 + (divs - 1) * 4) * divs]; 
    
        // create the connectivity lists                                                                                                                                                                        
        uint32_t vid = 1, numV = 0, l = 0; 
        k = 0; 
        for (uint32_t i = 0; i < divs; i++) { 
            for (uint32_t j = 0; j < divs; j++) { 
                if (i == 0) { 
                    faceIndex[k++] = 3; 
                    vertsIndex[l] = 0; 
                    vertsIndex[l + 1] = j + vid; 
                    vertsIndex[l + 2] = (j == (divs - 1)) ? vid : j + vid + 1; 
                    l += 3; 
                } 
                else if (i == (divs - 1)) { 
                    faceIndex[k++] = 3; 
                    vertsIndex[l] = j + vid + 1 - divs; 
                    vertsIndex[l + 1] = vid + 1; 
                    vertsIndex[l + 2] = (j == (divs - 1)) ? vid + 1 - divs : j + vid + 2 - divs; 
                    l += 3; 
                } 
                else { 
                    faceIndex[k++] = 4; 
                    vertsIndex[l] = j + vid + 1 - divs; 
                    vertsIndex[l + 1] = j + vid + 1; 
                    vertsIndex[l + 2] = (j == (divs - 1)) ? vid + 1 : j + vid + 2; 
                    vertsIndex[l + 3] = (j == (divs - 1)) ? vid + 1 - divs : j + vid + 2 - divs; 
                    l += 4; 
                } 
                numV++; 
            } 
            vid = numV; 
        } 
    
        return new PolygonMesh(npolys, faceIndex, vertsIndex, P, N, st); 
    } 
 
};



#endif