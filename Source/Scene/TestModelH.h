#ifndef TEST_MODEL_CORNEL_BOX_H
#define TEST_MODEL_CORNEL_BOX_H

// Defines a simple test model: The Cornel Box

#include <glm/glm.hpp>
#include <vector>
#include "../Shapes/triangle.h"
// Used to describe a triangular surface:


// Loads the Cornell Box. It is scaled to fill the volume:
// -1 <= x <= +1
// -1 <= y <= +1
// -1 <= z <= +1
void LoadTestModel( std::vector<Shape2D*>& triangles )
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

    vec3 lowGloss(0.7, 0.7, 0.7);
    vec3 normalGloss(1, 1, 1);
    vec3 someGloss(1.2, 1.2, 1.2);
    vec3 moreGloss(1.5, 1.5, 1.5);
    vec3 highGloss(2, 2, 2);
    vec3 superGloss(5, 5, 5);


    // ---------------------------------------------------------------------------
    // Room

    float L = 555;			// Length of Cornell Box side.

    vec4 A(L,0,0,1);
    vec4 B(0,0,0,1);
    vec4 C(L,0,L,1);
    vec4 D(0,0,L,1);

    vec4 E(L,L,0,1);
    vec4 F(0,L,0,1);
    vec4 G(L,L,L,1);
    vec4 H(0,L,L,1);

    // Floor:
    triangles.push_back( new Triangle( C, B, A, green , moreGloss) );
    triangles.push_back( new Triangle( C, D, B, green , moreGloss) );

    // Left wall
    triangles.push_back( new Triangle( A, E, C, purple, highGloss ) );
    triangles.push_back( new Triangle( C, E, G, purple, highGloss ) );

    // Right wall
    triangles.push_back( new Triangle( F, B, D, yellow, lowGloss ) );
    triangles.push_back( new Triangle( H, F, D, yellow, lowGloss ) );

    // Ceiling
    triangles.push_back( new Triangle( E, F, G, cyan , normalGloss) );
    triangles.push_back( new Triangle( F, H, G, cyan , normalGloss) );

    // Back wall
    triangles.push_back( new Triangle( G, D, C, white, someGloss ) );
    triangles.push_back( new Triangle( G, H, D, white, someGloss ) );

    // ---------------------------------------------------------------------------
    // Short block

    A = vec4(290,0,114,1);
    B = vec4(130,0, 65,1);
    C = vec4(240,0,272,1);
    D = vec4( 82,0,225,1);

    E = vec4(290,165,114,1);
    F = vec4(130,165, 65,1);
    G = vec4(240,165,272,1);
    H = vec4( 82,165,225,1);

    // Front
    triangles.push_back( new Triangle(E,B,A,red, someGloss) );
    triangles.push_back( new Triangle(E,F,B,red, someGloss) );

    // Front
    triangles.push_back( new Triangle(F,D,B,red, someGloss) );
    triangles.push_back( new Triangle(F,H,D,red, someGloss) );

    // BACK
    triangles.push_back( new Triangle(H,C,D,red, someGloss) );
    triangles.push_back( new Triangle(H,G,C,red, someGloss) );

    // LEFT
    triangles.push_back( new Triangle(G,E,C,red, someGloss) );
    triangles.push_back( new Triangle(E,A,C,red, someGloss) );

    // TOP
    triangles.push_back( new Triangle(G,F,E,red, superGloss) );
    triangles.push_back( new Triangle(G,H,F,red, superGloss) );

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
    triangles.push_back( new Triangle(E,B,A,blue, highGloss) );
    triangles.push_back( new Triangle(E,F,B,blue, highGloss) );

    // Front
    triangles.push_back( new Triangle(F,D,B,blue, highGloss) );
    triangles.push_back( new Triangle(F,H,D,blue, highGloss) );

    // BACK
    triangles.push_back( new Triangle(H,C,D,blue, highGloss) );
    triangles.push_back( new Triangle(H,G,C,blue, highGloss) );

    // LEFT
    triangles.push_back( new Triangle(G,E,C,blue, highGloss) );
    triangles.push_back( new Triangle(E,A,C,blue, highGloss) );

    // TOP
    triangles.push_back( new Triangle(G,F,E,blue, superGloss) );
    triangles.push_back( new Triangle(G,H,F,blue, superGloss) );


    // ----------------------------------------------
    // Scale to the volume [-1,1]^3


}

#endif
