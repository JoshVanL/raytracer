#ifndef SCENE_H
#define SCENE_H

// Defines a simple test model: The Cornel Box

#include <glm/glm.hpp>
#include <vector>
#include "../Shapes/triangle.h"
#include "../Shapes/sphere.h"
#include "../Materials/material.h"
#include "../Materials/translucent.h"
#include "../Materials/specular.h"
#include "../Materials/diffuse.h"
#include "../Materials/MaterialProperties/gloss.h"
// Used to describe a triangular surface:


// Loads the Cornell Box. It is scaled to fill the volume:
// -1 <= x <= +1
// -1 <= y <= +1
// -1 <= z <= +1
void LoadTestModel( std::vector<Shape2D*>& shapes )
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
    shapes.push_back( new Triangle( C, B, A, green ,  {new Specular()}));
    shapes.push_back( new Triangle( C, D, B, green ,  {new Specular()} ));

    // Left wall
    shapes.push_back( new Triangle( A, E, C, purple,  {new Specular()} ) );
    shapes.push_back( new Triangle( C, E, G, purple,  {new Specular()} ) );

    // Right wall
    shapes.push_back( new Triangle( F, B, D, yellow,  {new Specular()} ) );
    shapes.push_back( new Triangle( H, F, D, yellow,  {new Specular()} ) );

    // Ceiling
    shapes.push_back( new Triangle( E, F, G, cyan ,  {new Specular()}) );
    shapes.push_back( new Triangle( F, H, G, cyan ,  {new Specular()}) );

    // Back wall
    shapes.push_back( new Triangle( G, D, C, white,  {new Specular()}) );
    shapes.push_back( new Triangle( G, H, D, white,  {new Specular()} ) );

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
    shapes.push_back( new Triangle(E,B,A,red,  {new Translucent()} ));
    shapes.push_back( new Triangle(E,F,B,red,  {new Translucent()} ));

    // Front
    shapes.push_back( new Triangle(F,D,B,red,  {new Translucent()} ));
    shapes.push_back( new Triangle(F,H,D,red,  {new Translucent()} ));

    // BACK
    shapes.push_back( new Triangle(H,C,D,red,  {new Translucent()}));
    shapes.push_back( new Triangle(H,G,C,red,  {new Translucent()}));

    // LEFT
    shapes.push_back( new Triangle(G,E,C,red,  {new Translucent()}));
    shapes.push_back( new Triangle(E,A,C,red,  {new Translucent()}));

    // TOP
    shapes.push_back( new Triangle(G,F,E,red,  {new Translucent()}));
    shapes.push_back( new Triangle(G,H,F,red,  {new Translucent()}));

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
    shapes.push_back( new Triangle(E,B,A,blue, { new Translucent() }) );
    shapes.push_back( new Triangle(E,F,B,blue,  { new Translucent() }) );

    // Front
    shapes.push_back( new Triangle(F,D,B,blue,  { new Translucent() }) );
    shapes.push_back( new Triangle(F,H,D,blue,  { new Translucent() }) );

    // BACK
    shapes.push_back( new Triangle(H,C,D,blue,  { new Translucent() }) );
    shapes.push_back( new Triangle(H,G,C,blue,  { new Translucent() }) );

    // LEFT
    shapes.push_back( new Triangle(G,E,C,blue,  { new Translucent() }) );
    shapes.push_back( new Triangle(E,A,C,blue,  { new Translucent() }) );

    // TOP
    shapes.push_back( new Triangle(G,F,E,blue,  { new Translucent(), new Specular() }) );
    shapes.push_back( new Triangle(G,H,F,blue,  { new Translucent() }) );

    // ---------------------------------------------------------------------------
    // Sphere
    float radius = 100;
    float empty = 0;
    A = vec4(700, 300, 200, 1);
    vec3 emptyv3 =  vec3(0,0,0);
    shapes.push_back( new Sphere( A, radius, blue, { new Translucent() }) );
}
#endif
