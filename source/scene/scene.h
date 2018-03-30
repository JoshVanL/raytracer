#ifndef SCENE_H
#define SCENE_H

// Defines a simple test model: The Cornel Box
#include <fstream>
#include <glm/glm.hpp>
#include <vector>
#include <sstream>

#include "../shapes/triangle.h"
#include "../shapes/sphere.h"
#include "../shapes/rectangle.h"
#include "../shapes/wave.h"
#include "../materials/portal.h"
#include "../materials/material.h"
#include "../materials/translucent.h"
#include "../materials/specular.h"
#include "../materials/diffuse.h"
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
void LoadTestModel( std::vector<Shape2D*>& shapes )
{
   
    using glm::vec3; 
    using glm::vec4;

    // Defines colors:
    

    // ---------------------------------------------------------------------------
    // Room

    //float L = 1000;			// Length of Cornell Box side.

    //vec4 A(2.f*L,0,0,1);
    //vec4 B(0,0,0,1);
    //vec4 C(2.f*L,0,L,1);
    //vec4 D(0,0,L,1);

    //vec4 E(2.f*L,L,0,1);
    //vec4 F(0,L,0,1);
    //vec4 G(2.f*L,L,L,1);
    //vec4 H(0,L,L,1);

    float L = 555;

    vec4 A(L,0,0,1);
    vec4 B(0,0,0,1);
    vec4 C(L,0,L,1);
    vec4 D(0,0,L,1);

    vec4 E(L,L,0,1);
    vec4 F(0,L,0,1);
    vec4 G(L,L,L,1);
    vec4 H(0,L,L,1);

    // Mirror
    vec4 I(4,100,400,1);
    vec4 J(4,400,220,1);
    vec4 K(4,400,400,1);
    vec4 M(4,400,220,1);
    vec4 N(4,100,400,1);
    vec4 O(4,100,220,1);

    vec4 Ia(540,100,150,1);
    vec4 Ja(540,400,-30,1);
    vec4 Ka(540,400,150,1);
    vec4 Ma(540,400,-30,1);
    vec4 Na(540,100,150,1);
    vec4 Oa(540,100,-30,1);

    // Floor:
    shapes.push_back( new Triangle( C, B, A, white , "wall",   {new Texture("source/materials/textures/carpet.bmp")}));
    shapes.push_back( new Triangle( C, D, B, white , "wall",   {new Texture("source/materials/textures/carpet.bmp")} ));

    // Left wall
    shapes.push_back( new Triangle( A, E, C, white, "wall",   {new Texture("source/materials/textures/wall2.bmp") }) );
    shapes.push_back( new Triangle( C, E, G, white, "wall",   {new Texture("source/materials/textures/wall2.bmp") } ) );

    // Right wall
    shapes.push_back( new Triangle( F, B, D, white, "wall",   {new Texture("source/materials/textures/wall1.bmp")} ) );
    shapes.push_back( new Triangle( H, F, D, white, "wall",   {new Texture("source/materials/textures/wall1.bmp")} ) ); 

    // Ceiling
    shapes.push_back( new Triangle( E, F, G, white ,"wall",    {new Texture("source/materials/textures/ceil.bmp")}) );
    shapes.push_back( new Triangle( F, H, G, white , "wall",   {new Texture("source/materials/textures/ceil.bmp")}) );
    // Back wall
    shapes.push_back( new Triangle( G, D, C, white, "wall",   {new Texture("source/materials/textures/water.bmp")}) );
    shapes.push_back( new Triangle( G, H, D, white, "wall",   {new Texture("source/materials/textures/water.bmp")}) );

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

    //A = vec4(690,0,164,1);
    //B = vec4(530,0,115,1);
    //C = vec4(640,0,322,1);
    //D = vec4(482,0,275,1);

    //E = vec4(690,165,164,1);
    //F = vec4(530,165,115,1);
    //G = vec4(640,165,322,1);
    //H = vec4(482,165,275,1);

    A = vec4(290,0,114,1);
    B = vec4(130,0, 65,1);
    C = vec4(240,0,272,1);
    D = vec4( 82,0,225,1);

    E = vec4(290,165,114,1);
    F = vec4(130,165, 65,1);
    G = vec4(240,165,272,1);
    H = vec4( 82,165,225,1);

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

    //A = vec4(423,0,247,1);
    //B = vec4(265,0,296,1);
    //C = vec4(472,0,406,1);
    //D = vec4(314,0,456,1);

    //E = vec4(423,330,247,1);
    //F = vec4(265,330,296,1);
    //G = vec4(472,330,406,1);
    //H = vec4(314,330,456,1);

    A = vec4(423,0,247,1);
    B = vec4(265,0,296,1);
    C = vec4(472,0,406,1);
    D = vec4(314,0,456,1);

    E = vec4(423,330,247,1);
    F = vec4(265,330,296,1);
    G = vec4(472,330,406,1);
    H = vec4(314,330,456,1);

    // Front
    shapes.push_back( new Triangle(E,B,A,blue,"tallblock",  {  new Smoke(blue) }) );
    shapes.push_back( new Triangle(E,F,B,blue, "tallblock",  {  new Smoke(blue)}) );

    // Front
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
    A = vec4(200, 230, 150, 1);
    vec3 emptyv3 =  vec3(0,0,0);
    shapes.push_back( new Sphere( A, radius, blue, "sphere1", { new Translucent() }) );
    A = vec4(350, 400, 350, 1);
    radius = 70;
    shapes.push_back( new Sphere( A, radius ,red, "sphere2", { new Translucent(true, false), new Specular() }) );
}


void LoadTestModel2( std::vector<Shape2D*>& shapes )
{
    using glm::vec3; 
    using glm::vec4;

    // Defines colors:
    vec3 red(    0.75f, 0.15f, 0.15f );
    vec3 yellow( 0.75f, 0.75f, 0.15f );
    vec3 green(  0.15f, 0.75f, 0.15f );
    vec3 cyan(   0.15f, 0.75f, 0.75f );
    vec3 blue(   0.15f, 0.15f, 0.75f );
    vec3 purple( 0.75f, 0.15f, 0.75f );
    vec3 white(  0.75f, 0.75f, 0.75f );

    // ---------------------------------------------------------------------------
    // Room

    float L = 1000;			// Length of Cornell Box side.

    vec4 A(2.f*L,0,0,1);
    vec4 B(0,0,0,1);
    vec4 C(2.f*L,0,L,1);
    vec4 D(0,0,L,1);

    vec4 E(2.f*L,L,0,1);
    vec4 F(0,L,0,1);
    vec4 G(2.f*L,L,L,1);
    vec4 H(0,L,L,1);

    // Floor:
    shapes.push_back( new Triangle( C, B, A, green , "wall" ));
    shapes.push_back( new Triangle( C, D, B, green , "wall" ));

    // Left wall
    shapes.push_back( new Triangle( A, E, C, purple,"wall") );
    shapes.push_back( new Triangle( C, E, G, purple, "wall" ) );

    // Right wall
    shapes.push_back( new Triangle( F, B, D, yellow, "wall" ) );
    shapes.push_back( new Triangle( H, F, D, yellow, "wall" ) );

    // Ceiling
    shapes.push_back( new Triangle( E, F, G, cyan , "wall") );
    shapes.push_back( new Triangle( F, H, G, cyan , "wall") );

    // Back wall
    shapes.push_back( new Triangle( G, D, C, white, "wall") );
    shapes.push_back( new Triangle( G, H, D, white, "wall" ) );

    // ---------------------------------------------------------------------------
    // Short block

    A = vec4(690,0,164,1);
    B = vec4(530,0,115,1);
    C = vec4(640,0,322,1);
    D = vec4(482,0,275,1);

    E = vec4(690,165,164,1);
    F = vec4(530,165,115,1);
    G = vec4(640,165,322,1);
    H = vec4(482,165,275,1);

    // Front
    shapes.push_back( new Triangle(E,B,A,red, "sb",   {new Translucent()} ));
    shapes.push_back( new Triangle(E,F,B,red, "sb",  {new Translucent()} ));

    // Front
    shapes.push_back( new Triangle(F,D,B,red,"sb",   {new Translucent()} ));
    shapes.push_back( new Triangle(F,H,D,red, "sb",  {new Translucent()} ));

    // BACK
    shapes.push_back( new Triangle(H,C,D,red, "sb",  {new Translucent()}));
    shapes.push_back( new Triangle(H,G,C,red, "sb",  {new Translucent()}));

    // LEFT
    shapes.push_back( new Triangle(G,E,C,red,"sb",   {new Translucent()}));
    shapes.push_back( new Triangle(E,A,C,red,"sb",   {new Translucent()}));

    // TOP
    shapes.push_back( new Triangle(G,F,E,red, "sb",  {new Translucent()}));
    shapes.push_back( new Triangle(G,H,F,red,"sb",   {new Translucent()}));

    // ---------------------------------------------------------------------------
    // Tall block

    A = vec4(423,0,247,1);
    B = vec4(265,0,296,1);
    C = vec4(472,0,406,1);
    D = vec4(314,0,456,1);

    E = vec4(423,330,247,1);
    F = vec4(265,330,296,1);
    G = vec4(472,330,406,1);
    H = vec4(314,330,456,1);

    // Front
    shapes.push_back( new Triangle(E,B,A,blue,"tb",  { new Translucent() }) );
    shapes.push_back( new Triangle(E,F,B,blue,"tb",    { new Translucent() }) );

    // Front
    shapes.push_back( new Triangle(F,D,B,blue,"tb",    { new Translucent() }) );
    shapes.push_back( new Triangle(F,H,D,blue,"tb",    { new Translucent() }) );

    // BACK
    shapes.push_back( new Triangle(H,C,D,blue,"tb",    { new Translucent() }) );
    shapes.push_back( new Triangle(H,G,C,blue,"tb",    { new Translucent() }) );

    // LEFT
    shapes.push_back( new Triangle(G,E,C,blue,"tb",    { new Translucent() }) );
    shapes.push_back( new Triangle(E,A,C,blue,"tb",    { new Translucent() }) );

    // TOP
    shapes.push_back( new Triangle(G,F,E,blue,"tb",    { new Translucent(), new Specular() }) );
    shapes.push_back( new Triangle(G,H,F,blue,"tb",    { new Translucent() }) );

    // ---------------------------------------------------------------------------
    // Sphere
    float radius = 100;
    float empty = 0;
    A = vec4(700, 300, 200, 1);
    vec3 emptyv3 =  vec3(0,0,0);
    shapes.push_back( new Sphere( A, radius, blue,"sph1", { new Translucent() }) );
}
#endif
