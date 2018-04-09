#ifndef POLYMESH_H
#define POLYMESH_H 

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

class PolyMesh
{
public:
    PolyMesh() : vertices(nullptr), st(nullptr), normals(nullptr) {}

    static PolyMesh* createPolyMesh(   uint32_t width = 1,
                                uint32_t height = 1,
                                uint32_t subdivisionWidth = 40,
                                uint32_t subdivisionHeight = 40)
    {
        PolyMesh *poly          = new PolyMesh;
        poly->numVertices       = (subdivisionWidth + 1) * (subdivisionHeight + 1);
        poly->vertices          = new vec3[poly->numVertices];
        poly->normals           = new vec3[poly->numVertices];
        poly->st                = new vec2[poly->numVertices];
        float invSubdivisionWidth   = 1.f / subdivisionWidth;
        float invSubdivisionHeight  = 1.f / subdivisionHeight;
        
        for (uint32_t j = 0; j <= subdivisionHeight; ++j) {
            for (uint32_t i = 0; i <= subdivisionWidth; ++i) {
                poly->vertices[j * (subdivisionWidth + 1) + i] = vec3(width * (i * invSubdivisionWidth - 0.5), 0, height * (j * invSubdivisionHeight - 0.5));
                poly->st[j * (subdivisionWidth + 1) + i] = vec2(i * invSubdivisionWidth, j * invSubdivisionHeight);
            }
        }

        poly->numFaces = subdivisionWidth * subdivisionHeight;
        poly->faceArray = new uint32_t[poly->numFaces];
        for (uint32_t i = 0; i < poly->numFaces; ++i)
            poly->faceArray[i] = 4;

        poly->verticesArray = new uint32_t[4 * poly->numFaces];
        for (uint32_t j = 0, k = 0; j < subdivisionHeight; ++j) {
            for (uint32_t i = 0; i < subdivisionWidth; ++i) {
                poly->verticesArray[k] = j * (subdivisionWidth + 1) + i;
                poly->verticesArray[k + 1] = j * (subdivisionWidth + 1) + i + 1;
                poly->verticesArray[k + 2] = (j + 1) * (subdivisionWidth + 1) + i + 1;
                poly->verticesArray[k + 3] = (j + 1) * (subdivisionWidth + 1) + i;
                k += 4;
            }
        }

        return poly;
    }

    ~PolyMesh()
    {
        if (vertices) delete[] vertices;
        if (st) delete[] st;
        if (normals) delete[] normals;
    }
    vec3 *vertices;
    vec2 *st;
    vec3 *normals;
    uint32_t *faceArray;
    uint32_t *verticesArray;
    uint32_t numVertices;
    uint32_t numFaces;
};



#endif