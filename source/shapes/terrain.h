#ifndef TERRAIN_H
#define TERRAIN_H
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <glm/glm.hpp>
#include "shape2D.h"
#include "triangle.h"
#include <vector>
using std::vector;
using glm::vec4;
using glm::vec3;

class Terrain{
public:

    vector<vector<Triangle*> > triangles;
    float L = 25;
    int width = 10;
    int height = 10;
    vec3 green = vec3(  0.15f, 0.75f, 0.15f );
    Terrain(){
        vec4 A(0,0,L,  1);
        vec4 B(0,0,0,  1);
        vec4 C(L/2,0,L/2,1);

        vec4 D(0,0,0,1);
        vec4 E(L,0,0,1);
        vec4 F(L/2,0,L/2,1);

        vec4 G(L,0,0,1);
        vec4 H(L,0,L,1);
        vec4 I(L/2,0,L/2,1);

        vec4 J(L,0,L,1);
        vec4 K(0,0,L,1);
        vec4 O(L/2,0,L/2,1);

        float rnd;
        for(int i = 0; i < width; i ++){
            for(int j = 0; j < height; j++){
                vec4 test =  C + vec4(i*L,0,j*L,0);
                vector<Triangle*> triangleSet;
                rnd = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)));
                triangleSet.push_back( new Triangle( A + vec4(i*L,0,j*L,0), B+ vec4(i*L,0,j*L,0), C + vec4(i*L,0,j*L,0),  green * rnd , "wall"));
                rnd = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)));
                triangleSet.push_back( new Triangle( D + vec4(i*L,0,j*L,0), E+ vec4(i*L,0,j*L,0), F + vec4(i*L,0,j*L,0),  green * rnd , "wall" ));
                rnd = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)));
                triangleSet.push_back( new Triangle( G + vec4(i*L,0,j*L,0), H+ vec4(i*L,0,j*L,0), I + vec4(i*L,0,j*L,0),  green * rnd , "wall" ));
                rnd = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)));
                triangleSet.push_back( new Triangle( J + vec4(i*L,0,j*L,0), K+ vec4(i*L,0,j*L,0), O + vec4(i*L,0,j*L,0),  green * rnd , "wall" ));

                triangles.push_back(triangleSet);
            }
        }
        float scale = 0.99f;
        int step = 5;
        
        diamond(scale);
        diamond(step, scale);
        // square();
    }
    vec4 scalevec4(vec4 v) {
        v *= 2/L;
        v -= vec4(1,1,1,1);
        v.x *= -1;
        v.y *= -1;
        v.w = 1.0;
        return v;
    }
    void diamond(int step, float magnitude){
        std::random_device rd;     // only used once to initialise (seed) engine
        std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
        std::uniform_real_distribution<double> uni(0.90,1); // guaranteed unbiased

        int stepdouble = 4;
        float rnd;
        for(int i = 0; i < width; i ++){
            for(int j = 0; j < height; j++){
                glm::mat4 sca(1.f);
                vector<Triangle*> tris = triangles[i*width + j];
                sca[1][1] = ((tris[0]->v0.y + tris[0]->v1.y + tris[2]->v0.y + tris[2]->v1.y )/4.f) * uni(rng);//  * magnitude  ; 
                printf("%f \n", sca[1][1]);
                tris[0]->v2 = sca * tris[0]->v2;  
                tris[0]->v2.y =  tris[0]->v2.y * 1.f;     
                tris[1]->v2.y =  tris[0]->v2.y * 1.f;
                tris[2]->v2.y =  tris[0]->v2.y * 1.f;
                tris[3]->v2.y =  tris[0]->v2.y * 1.f;
                std::cout <<   tris[0]->v2.y  <<  tris[1]->v2.y   << tris[2]->v2.y   <<  tris[3]->v2.y  << "\n";
            }
        }
    }
    void diamond(float magnitude){
       std::random_device rd;     // only used once to initialise (seed) engine
        std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
        std::uniform_real_distribution<double> uni(0.90,1); // guaranteed unbiased

        int stepdouble = 4;
        float rnd;
        for(int i = 1; i < width-1; i ++){
            for(int j = 1; j < height-1; j++){
                glm::mat4 sca(1.f);
                vector<Triangle*> trisTL = triangles[i*width-2 + j - 1];
                vector<Triangle*> trisTR = triangles[i*width-2 + j + 1];
                vector<Triangle*> trisBL = triangles[i*width + j - 1];
                vector<Triangle*> trisBR = triangles[i*width + j + 1];
                vector<Triangle*> tris = triangles[i*4 + j];
                sca[1][1] = ((trisTL[0]->v0.y + trisTR[0]->v1.y + trisBL[2]->v0.y + trisBR[2]->v1.y )/4.f) * uni(rng);

                tris[0]->v2 = sca * tris[0]->v2;  
                tris[0]->v2.y =  tris[0]->v2.y * 1.f;     
                tris[1]->v2.y =  tris[0]->v2.y * 1.f;
                tris[2]->v2.y =  tris[0]->v2.y * 1.f;
                tris[3]->v2.y =  tris[0]->v2.y * 1.f;
                std::cout <<   tris[0]->v2.y  <<  tris[1]->v2.y   << tris[2]->v2.y   <<  tris[3]->v2.y  << "\n";
            }
        }
    }
    void square(){
        std::random_device rd;     // only used once to initialise (seed) engine
        std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
        std::uniform_real_distribution<double> uni(0.90,1); // guaranteed unbiased

        int stepdouble = 4;
        float rnd;
        for(int i = 1; i < 5; i ++){
            for(int j = 1; j < 5; j++){
                glm::mat4 sca(1.f);
                vector<Triangle*> trisBL = triangles[i*3 + j - 1];
                vector<Triangle*> trisBR = triangles[i*3 + j ];
                vector<Triangle*> trisTL = triangles[i*4 + j - 1];
                vector<Triangle*> trisTR = triangles[i*4 + j];
                sca[1][1] = ((trisTL[0]->v0.y + trisTR[0]->v1.y + trisBL[2]->v0.y + trisBR[2]->v1.y )/4.f) * uni(rng);

                trisBR[0]->v1 = sca * trisBR[0]->v1;      
                trisBR[1]->v0 = sca * trisBR[0]->v0;  
                //////////////
                trisBL[2]->v0 = sca * trisBL[2]->v0;
                trisBL[1]->v1 =  sca * trisBL[1]->v1;
                /////////////
                trisTL[2]->v1 = sca * trisTL[2]->v1;
                trisTL[3]->v0 = sca * trisTL[3]->v0;
                //////////////
                trisTR[3]->v1 = sca * trisTR[3]->v1;
                trisTR[0]->v0 = sca * trisTR[0]->v0;
                /////////////

            }
        }
    }



};

#endif