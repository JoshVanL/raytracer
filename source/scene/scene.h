#ifndef SCENE_H
#define SCENE_H

// Defines a simple test model: The Cornel Box
#include <fstream>
#include <glm/glm.hpp>
#include <vector>
#include <sstream>
#include "perlinnoise.h"
#include "../light/pointlight.h"
#include "../shapes/triangle.h"
#include "../shapes/terrain.h"
#include "../shapes/sphere.h"
#include "../shapes/rectangle.h"
#include "../shapes/cloud.h"
#include "../materials/portal.h"
#include "../materials/material.h"
#include "../materials/translucent.h"
#include "../materials/specular.h"
#include "../materials/diffuse.h"
#include "../materials/luminous.h"
#include "../materials/texture.h"
#include "../materials/smoke.h"
#include "../materials/mirror.h"
#include "../materials/material_properties/gloss.h"
// Used to describe a triangular surface:


// Loads the Cornell Box. It is scaled to fill the volume:
// -1 <= x <= +1
// -1 <= y <= +1
// -1 <= z <= +1
using glm::vec3;
vec3 red(    0.75f, 0.15f, 0.15f );
vec3 yellow( 0.75f, 0.75f, 0.15f );
vec3 green(  0.15f, 0.75f, 0.15f );
vec3 cyan(   0.15f, 0.75f, 0.75f );
vec3 blue(   0.15f, 0.15f, 0.75f );
vec3 purple( 0.75f, 0.15f, 0.75f );
vec3 white(  0.6f, 0.6f, 0.6f );
vec3 darkcyan(   0.075f, 0.375f, 0.375f );
vec3 lightblue(   0.5f, 0.5f, 0.4f );

void readMeshData(std::string filepath, std::vector<Shape2D*>& shapes){
    std::ifstream infile(filepath);
    std::string line;
    if(!infile){
        printf("nope\n");
    }
    int counter = 0;
    vec4 v0, v1, v2;
    while(std::getline(infile, line)){
        float a, b, c;
        std::istringstream iss(line);
        if(!(iss >> a >> b >> c)){
            break;
        }
        if(counter == 0){
            v0 = vec4(a, b, c,1);
            counter++;
        }
        else if(counter == 1){
            v1 = vec4(a, b, c,1);
            counter++;
        }
        else if(counter == 2){
            v2 = vec4(a, b, c,1);
            shapes.push_back( new Triangle( v0, v1, v2, green ,filepath, true, {new Specular()}));
            counter = 0;
        }
    }
}

vec4 scalevec4(vec4 v) {
        v *= 2/555;
        v -= vec4(1,1,1,1);
        v.x *= -1;
        v.y *= -1;
        v.w = 1.0;
        return v;
}
vec3 scalevec3(vec3 v) {
    v *= 2/555;
    v -= vec3(1,1,1);
    v.x *= -1;
    v.y *= -1;
    return v;
}
float scalefloat(float v) {
    v *= 2/555;
    return v;
}
void LoadTest(std::vector<Shape2D*>& shapes){
    using glm::vec3; 
    using glm::vec4;
    float L = 555;
    vec4 A(L,0,0,1);
    vec4 B(0,0,0,1);
    vec4 C(L,0,L,1);
    vec4 D(0,0,L,1);

    shapes.push_back( new Rectangle(A, B, C, D, white , "wall", {new Specular()}));
}

void LoadTestModel( std::vector<Shape2D*>& shapes, std::vector<LightSource*>& lights  )
{
   
    using glm::vec3; 
    using glm::vec4;

    // Defines colors:
    




    LightSource* lightA = new PointLight();
    LightSource* lightB = new PointLight(vec4(1, 1.5, -3, 1), vec3(1,1,1), 10.f);
    lights.push_back(lightB);
    lights.push_back(lightA);


    // ---------------------------------------------------------------------------
    // Room

    float L = 555;

    float** displacement = PerlinNoise::genHeightMap();
    // vec4 cloud_A(2.5*L, -500, 1000, 1);
    // vec4 cloud_B(2.5*L, 1000, 1000, 1);
    // vec4 cloud_C(-L, -500, 1000, 1);
    // vec4 cloud_D(-L, 1000, 1000, 1);

    // Cloud* cloud = new Cloud(displacement, 0, 200, 200, cloud_B, white);
    // shapes.push_back(cloud);
    // return;
    vec4 terrain_A(2.75*L ,0,-800,1);
    vec4 terrain_B(-1*L ,0,-800,1);
    vec4 terrain_C(2.75*L ,0,L-400,1);
    vec4 terrain_D(-1*L ,0,L-400,1);

    vec4 ocean_A(8*L/2,0,L-400,1);
    vec4 ocean_B(-6*L/2,0,L-400,1);
    vec4 ocean_C(8*L/2,0,L+800,1);
    vec4 ocean_D(-6*L/2,0,L+800,1);

    Terrain* terrain = new Terrain(displacement, 1500, 1500, terrain_B, terrain_A, terrain_D, terrain_C, vec3(0.1f, 0.2f,0.1f));
    Terrain* ocean = new Terrain(displacement, 1024, 1024, ocean_B, ocean_A, ocean_D, ocean_C, vec3(0.4f, 1.f, 0.8f), true);
    shapes.push_back(terrain);    
    shapes.push_back(ocean);

    vec4 A(L,0+500,0,1);
    vec4 B(0,0+500,0,1);
    vec4 C(L,0+500,L,1);
    vec4 D(0,0+500,L,1);

    vec4 E(L,L+500,0,1);
    vec4 F(0,L+500,0,1);
    vec4 G(L,L+500,L,1);
    vec4 H(0,L+500,L,1);   

    // Mirror
    vec4 I(4,100+500,400,1);
    vec4 J(4,400+500,220,1);
    vec4 K(4,400+500,400,1);
    vec4 M(4,400+500,220,1);
    vec4 N(4,100+500,400,1);
    vec4 O(4,100+500,220,1);

    vec4 Ia(540,100+500,200,1);
    vec4 Ja(540,400+500,20,1);
    vec4 Ka(540,400+500,200,1);
    vec4 Ma(540,400+500,20,1);
    vec4 Na(540,100+500,200,1);
    vec4 Oa(540,100+500,20,1);
    
    // Floor:
    shapes.push_back( new Triangle( C, B, A, white , "wall",   {new Texture("source/materials/textures/carpet.bmp",true)}));
    shapes.push_back( new Triangle( C, D, B, white , "wall",   {new Texture("source/materials/textures/carpet.bmp",true)} ));

    // Left wall
    shapes.push_back( new Triangle( A, E, C, white, "wall",   {new Texture("source/materials/textures/wall2.bmp",false) }) );
    shapes.push_back( new Triangle( C, E, G, white, "wall",   {new Texture("source/materials/textures/wall2.bmp",false) } ) );

    // Right wall
    shapes.push_back( new Triangle( F, B, D, white, "wall",   {new Texture("source/materials/textures/wall1.bmp",false)} ) );
    shapes.push_back( new Triangle( H, F, D, white, "wall",   {new Texture("source/materials/textures/wall1.bmp",false)} ) ); 

    // Ceiling
    shapes.push_back( new Triangle( E, F, G, white ,"wall",    {new Texture("source/materials/textures/ceil.bmp",true)}) );
    shapes.push_back( new Triangle( F, H, G, white , "wall",   {new Texture("source/materials/textures/ceil.bmp",true)}) );
    // Back wall
    shapes.push_back( new Triangle( G, D, C, white, "wall",   {new Texture("source/materials/textures/water.bmp",false)}) );
    shapes.push_back( new Triangle( G, H, D, white, "wall",   {new Texture("source/materials/textures/water.bmp",false)}) );

    A = vec4(450,0+500,270,1);
    B = vec4(330,0+500,150,1);
    C = vec4(450,0+500,150,1);
    D = vec4(330,0+500,270,1);
    E = vec4(390,150+500,210,1);
    shapes.push_back( new Triangle(A,C,E,green, "pyramid",  {new Translucent()}));
    shapes.push_back( new Triangle(C,B,E,red, "pyramid",  {new Smoke(red)}));
    shapes.push_back( new Triangle(B,D,E,green, "pyramid",  {new Translucent()}));
    shapes.push_back( new Triangle(D,A,E,green, "pyramid",  {new Translucent()}));

    // // ---------------------------------------------------------------------------
    // // Mirror
    Triangle* mirror_a = new Triangle( I, J, K, white, "mirror");
    Triangle* mirror_b = new Triangle( M, N, O, white, "mirror");
    Triangle* mirror_c = new Triangle( Ia, Ja, Ka, white, "mirror");
    Triangle* mirror_d = new Triangle( Ma, Na, Oa, white, "mirror");
    Portal* gate = new Portal(mirror_a, mirror_b, mirror_c, mirror_d);
    mirror_a->addMaterial(gate);
    mirror_b->addMaterial(gate);
    mirror_c->addMaterial(gate);
    mirror_d->addMaterial(gate);

    shapes.push_back(mirror_a );
    shapes.push_back( mirror_b);
    shapes.push_back(mirror_c );
    shapes.push_back( mirror_d);
    
    

    // ---------------------------------------------------------------------------
    // Short block

    A = vec4(290,0+500,114,1);
    B = vec4(130,0+500, 65,1);
    C = vec4(240,0+500,272,1);
    D = vec4( 82,0+500,225,1);

    E = vec4(290,165+500,114,1);
    F = vec4(130,165+500, 65,1);
    G = vec4(240,165+500,272,1);
    H = vec4( 82,165+500,225,1);

    // Front
    shapes.push_back( new Triangle(E,B,A,red, "shortblock",  {new Smoke(red)} ));
    shapes.push_back( new Triangle(E,F,B,red, "shortblock",  {new Smoke(red)} ));

    // // Front
    shapes.push_back( new Triangle(F,D,B,red,"shortblock",   {new Translucent()} ));
    shapes.push_back( new Triangle(F,H,D,red, "shortblock",  {new Translucent()} ));

    // // BACK
    shapes.push_back( new Triangle(H,C,D,red, "shortblock",  {new Translucent()}));
    shapes.push_back( new Triangle(H,G,C,red, "shortblock",  {new Translucent()}));

    // LEFT
    shapes.push_back( new Triangle(G,E,C,red, "shortblock",  {new Translucent()}));
    shapes.push_back( new Triangle(E,A,C,red,"shortblock",   {new Translucent()}));

    // TOP
    shapes.push_back( new Triangle(G,F,E,red, "shortblock",  {new Translucent()}));
    shapes.push_back( new Triangle(G,H,F,red, "shortblock",  {new Translucent()}));

    // // ---------------------------------------------------------------------------
    // // Tall block

    A = vec4(423,0+500,247,1);
    B = vec4(265,0+500,296,1);
    C = vec4(472,0+500,406,1);
    D = vec4(314,0+600,456,1);

    E = vec4(423,330+500,247,1);
    F = vec4(265,330+500,296,1);
    G = vec4(472,330+500,406,1);
    H = vec4(314,330+500,456,1);

    // Front
    shapes.push_back( new Triangle(E,B,A,blue,"tallblock",  {  new Smoke(blue) }) );
    shapes.push_back( new Triangle(E,F,B,blue, "tallblock",  {  new Smoke(blue)}) );

    // RIGHT
    shapes.push_back( new Triangle(F,D,B,blue, "tallblock",  { new Translucent() }) );
    shapes.push_back( new Triangle(F,H,D,blue,"tallblock",   { new Translucent() }) );

    // BACK
    shapes.push_back( new Triangle(H,C,D,blue,"tallblock",   { new Translucent() }) );
    shapes.push_back( new Triangle(H,G,C,blue, "tallblock",  { new Translucent() }) );

    // LEFT
    shapes.push_back( new Triangle(G,E,C,blue, "tallblock",  { new Translucent() }) );
    shapes.push_back( new Triangle(E,A,C,blue, "tallblock",  { new Translucent() }) );

    // // TOP
    shapes.push_back( new Triangle(G,F,E,blue, "tallblock",  { new Translucent() }) );
    shapes.push_back( new Triangle(G,H,F,blue, "tallblock",  { new Translucent() }) );

    // ---------------------------------------------------------------------------
    // Sphere
    float radius = 60;
    float empty = 0;
    A = vec4(200, 230+500, 150, 1);
    vec3 emptyv3 =  vec3(0,0,0);
    shapes.push_back( new Sphere( A, radius, blue, "sphere1", { new Translucent() }) );
    A = vec4(350, 400+500, 350, 1);
    radius = 70;
    shapes.push_back( new Sphere( A, radius ,red, "sphere2", { new Translucent(true, false), new Specular() }) );



}

#endif
