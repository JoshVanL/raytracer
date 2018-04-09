#ifndef PERLIN_H
#define PERLIN_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdio>
#include <random>
#include <functional>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <glm/glm.hpp>
#include "../shapes/polymesh.h"
using glm::vec2;
using glm::vec3;
using glm::vec4;


inline
float smoothstep(const float &t)
{
    return t * t * (3 - 2 * t);
}

inline
float quintic(const float &t)
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

inline
float smoothstepDeriv(const float &t)
{
    return t * (6 - 6 * t);
}

inline
float quinticDeriv(const float &t)
{
    return 30 * t * t * (t * (t - 2) + 1);
}

class PerlinNoise
{
public:
    PerlinNoise(const unsigned &seed = 2016)
    {
        std::mt19937 generator(seed);
        std::uniform_real_distribution<float> distribution;
        auto dice = std::bind(distribution, generator);
        for (unsigned i = 0; i < tableSize; ++i) {
            float theta = acos(2 * dice() - 1);
            float phi = 2 * dice() * M_PI;

            float x = cos(phi) * sin(theta);
            float y = sin(phi) * sin(theta);
            float z = cos(theta);
            gradients[i] = vec3(x, y, z);
            permutationTable[i] = i;
        }

        std::uniform_int_distribution<unsigned> distributionInt;
        auto diceInt = std::bind(distributionInt, generator);
        // create permutation table
        for (unsigned i = 0; i < tableSize; ++i)
            std::swap(permutationTable[i], permutationTable[diceInt() & tableSizeMask]);
        // extend the permutation table in the index range [256:512]
        for (unsigned i = 0; i < tableSize; ++i) {
            permutationTable[tableSize + i] = permutationTable[i];
        }
    }

    float eval(const vec3 &p, vec3& derivs) const 
    {
        int xi0 = ((int)std::floor(p.x)) & tableSizeMask;
        int yi0 = ((int)std::floor(p.y)) & tableSizeMask;
        int zi0 = ((int)std::floor(p.z)) & tableSizeMask;

        int xi1 = (xi0 + 1) & tableSizeMask;
        int yi1 = (yi0 + 1) & tableSizeMask;
        int zi1 = (zi0 + 1) & tableSizeMask;

        float tx = p.x - ((int)std::floor(p.x));
        float ty = p.y - ((int)std::floor(p.y));
        float tz = p.z - ((int)std::floor(p.z));

        float u = quintic(tx);
        float v = quintic(ty);
        float w = quintic(tz);

        // generate vectors going from the grid points to p
        float x0 = tx, x1 = tx - 1;
        float y0 = ty, y1 = ty - 1;
        float z0 = tz, z1 = tz - 1;

        float a = gradientDotV(hash(xi0, yi0, zi0), x0, y0, z0);
        float b = gradientDotV(hash(xi1, yi0, zi0), x1, y0, z0);
        float c = gradientDotV(hash(xi0, yi1, zi0), x0, y1, z0);
        float d = gradientDotV(hash(xi1, yi1, zi0), x1, y1, z0);
        float e = gradientDotV(hash(xi0, yi0, zi1), x0, y0, z1);
        float f = gradientDotV(hash(xi1, yi0, zi1), x1, y0, z1);
        float g = gradientDotV(hash(xi0, yi1, zi1), x0, y1, z1);
        float h = gradientDotV(hash(xi1, yi1, zi1), x1, y1, z1);

        float du = quinticDeriv(tx);
        float dv = quinticDeriv(ty);
        float dw = quinticDeriv(tz);

        float k0 = a;
        float k1 = (b - a);
        float k2 = (c - a);
        float k3 = (e - a);
        float k4 = (a + d - b - c);
        float k5 = (a + f - b - e);
        float k6 = (a + g - c - e);
        float k7 = (b + c + e + h - a - d - f - g);

        derivs.x = du *(k1 + k4 * v + k5 * w + k7 * v * w);
        derivs.y = dv *(k2 + k4 * u + k6 * w + k7 * v * w);
        derivs.z = dw *(k3 + k5 * u + k6 * v + k7 * v * w);

        return k0 + k1 * u + k2 * v + k3 * w + k4 * u * v + k5 * u * w + k6 * v * w + k7 * u * v * w;
    }


    static float** genHeightMap()
    {
        PerlinNoise noise;

        PolyMesh *poly = PolyMesh::createPolyMesh(3, 3, 30, 30);

        vec3 derivs;
        for (uint32_t i = 0; i < poly->numVertices; ++i) {
            vec3 p((poly->vertices[i].x + 0.5), 0, (poly->vertices[i].z + 0.5));
            poly->vertices[i].y = noise.eval(p, derivs);
            vec3 tangent(1, derivs.x, 0); 
            vec3 bitangent(0, derivs.z, 1); 
            poly->normals[i] = glm::normalize(vec3(-derivs.x, 1, -derivs.z));
        }
        delete poly;

        const uint32_t width = 2048, height = 2048;
        float **heightMap = (float**) malloc(width*height*sizeof(float*)); 

        for (uint32_t j = 0; j < height; ++j) {
            heightMap[j] = (float*) malloc(width*sizeof(float)); 
            for (uint32_t i = 0; i < width; ++i) {
                heightMap[j][i] = (noise.eval(vec3(i, 0, j) * (1.f/ 64.f), derivs) + 1) * 0.5;
            }
        }

        return heightMap;
    }

    uint8_t hash(const int &x, const int &y, const int &z) const
    {
        return permutationTable[permutationTable[permutationTable[x] + y] + z];
    }

    // Compute dot product between vector from cell corners to P with predefined gradient directions
    //    perm: a value between 0 and 255
    //    float x, float y, float z: coordinates of vector from cell corner to shaded point
    float gradientDotV( uint8_t perm, // a value between 0 and 255
                        float x, float y, float z) const
    {
        switch (perm & 15) {
            case  0: return  x + y; // (1,1,0)
            case  1: return -x + y; // (-1,1,0)
            case  2: return  x - y; // (1,-1,0)
            case  3: return -x - y; // (-1,-1,0)
            case  4: return  x + z; // (1,0,1)
            case  5: return -x + z; // (-1,0,1)
            case  6: return  x - z; // (1,0,-1)
            case  7: return -x - z; // (-1,0,-1)
            case  8: return  y + z; // (0,1,1),
            case  9: return -y + z; // (0,-1,1),
            case 10: return  y - z; // (0,1,-1),
            case 11: return -y - z; // (0,-1,-1)
            case 12: return  y + x; // (1,1,0)
            case 13: return -x + y; // (-1,1,0)
            case 14: return -y + z; // (0,-1,1)
            case 15: return -y - z; // (0,-1,-1)
        }
    }

private:
   
    static const unsigned tableSize = 256;
    static const unsigned tableSizeMask = tableSize - 1;
    vec3 gradients[tableSize];
    unsigned permutationTable[tableSize * 2];
};

#endif