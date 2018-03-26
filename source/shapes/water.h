#ifndef WATER_H
#define WATER_H
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <glm/glm.hpp>
#include "shape2D.h"
#include "../materials/translucent.h"
#include "triangle.h"
#include <vector>
using std::vector;
using glm::vec4;
using glm::vec3;

class Water{
public:

    vector<vector<Triangle*> > triangles;
    float L = 50;
    int width = 10;
    int height = 10;
    vec3 green = vec3(  1.f, 1.f, 1.0f );
    Water(){
        //Squares are pushed from right to left, bottom to top
        // RIGHT TRIANGLE
        vec4 A(0, 0, L,  1);        //TOP-RIGHT CORNER     
        vec4 B(0, 0, 0,  1);        //BOTTOM-RIGHT CORNER    
        vec4 C(L/2,  0, L/2, 1);    //MIDDLE
        // BOTTOM TRIANGLE
        vec4 D(0, 0,0,1);           //BOTTOM-RIGHT CORNER
        vec4 E(L, 0,0,1);           //BOTTOM-LEFT CORNER
        vec4 F(L/2,0,L/2,1);        //MIDDLE
        // LEFT TRIANGLE
        vec4 G(L,0,0,1);            //BOTTOM-LEFT CORNER
        vec4 H(L,0,L,1);            //TOP-LEFT CORNER
        vec4 I(L/2,0,L/2,1);        //CENTER
        // TOP TRIANGLE
        vec4 J(L,0,L,1);            //TOP-LEFT CORNER
        vec4 K(0,0,L,1);            //TOP-RIGHT CORNER
        vec4 O(L/2,0,L/2,1);        //CENTER

        float rnd;
        for(int i = 0; i < width; i ++){
            for(int j = 0; j < height; j++){
                vec4 test =  C + vec4(i*L,0,j*L,0);
                vector<Triangle*> triangleSet;
                rnd = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)));
                triangleSet.push_back( new Triangle( A + vec4(i*L,0,j*L,0), B+ vec4(i*L,0,j*L,0), C + vec4(i*L,0,j*L,0),  green , "wall", {new Texture("source/materials/textures/water.bmp"), new Translucent()}));
                rnd = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)));
                triangleSet.push_back( new Triangle( D + vec4(i*L,0,j*L,0), E+ vec4(i*L,0,j*L,0), F + vec4(i*L,0,j*L,0),  green  , "wall",{new Texture("source/materials/textures/water.bmp"), new Translucent()} ));
                rnd = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)));
                triangleSet.push_back( new Triangle( G + vec4(i*L,0,j*L,0), H+  vec4(i*L,0,j*L,0), I + vec4(i*L,0,j*L,0),  green  , "wall",{new Texture("source/materials/textures/water.bmp"), new Translucent()} ));
                rnd = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)));
                triangleSet.push_back( new Triangle( J + vec4(i*L,0,j*L,0), K + vec4(i*L,0,j*L,0), O + vec4(i*L,0,j*L,0),  green , "wall" ,{new Texture("source/materials/textures/water.bmp"), new Translucent()}));

                triangles.push_back(triangleSet);
            }
        }
        float scale = 0.99f;
        int step = 5;


        std::random_device rd;     // only used once to initialise (seed) engine
        std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
        std::uniform_real_distribution<double> uni(0.90,1); // guaranteed unbiased 

        diamond(0);
        square(0);
         diamond(1);
        square(1);
    }  
    void square(int offset){
        std::random_device rd;     // only used once to initialise (seed) engine
        std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
        std::uniform_real_distribution<double> uni(0.992,1); // guaranteed unbiased

        int stepdouble = 4;
        float rnd;
        for(int i = offset; i < width-1; i +=2){
            for(int j = offset; j < height-1; j+=2){
                glm::mat4 sca(1.f);
                vector<Triangle*> trisBR = triangles[i*(width) + j];
                vector<Triangle*> trisBL = triangles[(i+1)*(width) + j];
                vector<Triangle*> trisTR = triangles[(i)*(width) + j + 1];
                vector<Triangle*> trisTL = triangles[(i+1)*(width) + j + 1];
                sca[1][1] = sca[1][1] = ((trisBR[1]->v1.y + trisBR[0]->v0.y + trisTR[2]->v1.y + trisTR[3]->v1.y )/4.f) * uni(rng);
                trisTR[1]->v1 = sca * trisTR[1]->v1;      
                trisTR[2]->v0.y = trisTR[1]->v1.y;  
                //////////////
                trisBR[2]->v1.y = trisTR[1]->v1.y;  
                trisBR[3]->v0.y = trisTR[1]->v1.y;  
                /////////////
                trisTL[0]->v1.y = trisTR[1]->v1.y;  
                trisTL[1]->v0.y = trisTR[1]->v1.y;  
                // //////////////
                trisBL[0]->v0.y = trisTR[1]->v1.y;  
                trisBL[3]->v1.y = trisTR[1]->v1.y;  

            }
        }
    }


    vec4 scalevec4(vec4 v) {
        v *= 2/L;
        v -= vec4(1,1,1,1);
        v.x *= -1;
        v.y *= -1;
        v.w = 1.0;
        return v;
    }
    void diamond(int offset){
        std::random_device rd;     // only used once to initialise (seed) engine
        std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
        std::uniform_real_distribution<double> uni(0.992,1); // guaranteed unbiased

        float rnd;
        for(int i = offset; i < width; i +=2){
            for(int j = offset; j < height; j+=2){
                glm::mat4 sca(1.f);
                vector<Triangle*> tris = triangles[i*width + j];
                sca[1][1] = ((tris[0]->v0.y + tris[0]->v1.y + tris[2]->v0.y + tris[2]->v1.y )/4.f) * uni(rng);//  * magnitude  ; 
                tris[0]->v2 = sca * tris[0]->v2;  
                tris[0]->v2.y =  tris[0]->v2.y * 1.f;     
                tris[1]->v2.y =  tris[0]->v2.y * 1.f;
                tris[2]->v2.y =  tris[0]->v2.y * 1.f;
                tris[3]->v2.y =  tris[0]->v2.y * 1.f;
            }
        }
    }
};

#endif 