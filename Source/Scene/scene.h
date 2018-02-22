#ifndef SCENE_H
#define SCENE_H

// Defines a simple test model: The Cornel Box

#include <glm/glm.hpp>
#include <vector>
#include "../Shapes/triangle.h"
#include "../Shapes/sphere.h"
#include "../Materials/material.h"
#include "../Materials/solid.h"
#include "../Materials/translucent.h"
#include "../Materials/reflective.h"
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

    vec3 lowGloss(0.7, 0.7, 0.7);
    vec3 normalGloss(1, 1, 1);
    vec3 someGloss(1.2, 1.2, 1.2);
    vec3 moreGloss(1.5, 1.5, 1.5);
    vec3 highGloss(2, 2, 2);
    vec3 superGloss(5, 5, 5);


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
    shapes.push_back( new Triangle( C, B, A, green , moreGloss) );
    shapes.push_back( new Triangle( C, D, B, green , moreGloss) );

    // Left wall
    shapes.push_back( new Triangle( A, E, C, purple, highGloss ) );
    shapes.push_back( new Triangle( C, E, G, purple, highGloss ) );

    // Right wall
    shapes.push_back( new Triangle( F, B, D, yellow, lowGloss ) );
    shapes.push_back( new Triangle( H, F, D, yellow, lowGloss ) );

    // Ceiling
    shapes.push_back( new Triangle( E, F, G, cyan , normalGloss) );
    shapes.push_back( new Triangle( F, H, G, cyan , normalGloss) );

    // Back wall
    shapes.push_back( new Triangle( G, D, C, white, someGloss ) );
    shapes.push_back( new Triangle( G, H, D, white, someGloss ) );

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
    shapes.push_back( new Triangle(E,B,A,red, normalGloss ));
    shapes.push_back( new Triangle(E,F,B,red, normalGloss ));

    // Front
    shapes.push_back( new Triangle(F,D,B,red, someGloss ));
    shapes.push_back( new Triangle(F,H,D,red, someGloss ));

    // BACK
    shapes.push_back( new Triangle(H,C,D,red, normalGloss));
    shapes.push_back( new Triangle(H,G,C,red, normalGloss));

    // LEFT
    shapes.push_back( new Triangle(G,E,C,red, someGloss));
    shapes.push_back( new Triangle(E,A,C,red, someGloss));

    // TOP
    shapes.push_back( new Triangle(G,F,E,red, normalGloss));
    shapes.push_back( new Triangle(G,H,F,red, normalGloss));

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
    shapes.push_back( new Triangle(E,B,A,blue, highGloss) );
    shapes.push_back( new Triangle(E,F,B,blue, highGloss) );

    // Front
    shapes.push_back( new Triangle(F,D,B,blue, someGloss) );
    shapes.push_back( new Triangle(F,H,D,blue, someGloss) );

    // BACK
    shapes.push_back( new Triangle(H,C,D,blue, highGloss) );
    shapes.push_back( new Triangle(H,G,C,blue, highGloss) );

    // LEFT
    shapes.push_back( new Triangle(G,E,C,blue, highGloss) );
    shapes.push_back( new Triangle(E,A,C,blue, highGloss) );

    // TOP
    shapes.push_back( new Triangle(G,F,E,blue, normalGloss) );
    shapes.push_back( new Triangle(G,H,F,blue, normalGloss) );

    // ---------------------------------------------------------------------------
    // Sphere
    float radius = 100;
    float empty = 0;
    A = vec4(320, 300, 200, 1);
    vec3 emptyv3 =  vec3(0,0,0);
    vec3 ggg = vec3(1,1,1);
    shapes.push_back( new Sphere( A, radius, blue, ggg, new Reflective()));
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

    vec3 lowGloss(0.7, 0.7, 0.7);
    vec3 normalGloss(1, 1, 1);
    vec3 someGloss(1.2, 1.2, 1.2);
    vec3 moreGloss(1.5, 1.5, 1.5);
    vec3 highGloss(2, 2, 2);
    vec3 superGloss(5, 5, 5);


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

    vec4 I(L/3,L/2,L/2,1);
    vec4 J(L/3,0,L/2,1);
    vec4 K(L/4,L/2,L/2,1);
    vec4 M(L/4,0,L/2,1);

    // // Floor:
    shapes.push_back( new Triangle( C, B, A, green , highGloss ) );
    shapes.push_back( new Triangle( C, D, B, green , highGloss) );

    // Left wall
    shapes.push_back( new Triangle( A, E, C, purple, highGloss ) );
    shapes.push_back( new Triangle( C, E, G, purple, highGloss ) );

    // Right wall
    shapes.push_back( new Triangle( F, B, D, yellow, lowGloss ) );
    shapes.push_back( new Triangle( H, F, D, yellow, lowGloss ) );

    // Ceiling
    shapes.push_back( new Triangle( E, F, G, cyan , normalGloss) );
    shapes.push_back( new Triangle( F, H, G, cyan , normalGloss) );

    // Back wall
    shapes.push_back( new Triangle( G, D, C, white, someGloss ) );
    shapes.push_back( new Triangle( G, H, D, white, someGloss ) );
    // Back wall
    shapes.push_back( new Triangle( J, M, K, white, someGloss , new Reflective()) );
    shapes.push_back( new Triangle( M, K, I, white, someGloss , new Reflective()) );
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
    shapes.push_back( new Triangle(E,B,A,red, someGloss));
    shapes.push_back( new Triangle(E,F,B,red, someGloss));

    // Front
    shapes.push_back( new Triangle(F,D,B,red, someGloss ));
    shapes.push_back( new Triangle(F,H,D,red, someGloss ));

    // BACK
    shapes.push_back( new Triangle(H,C,D,red, someGloss));
    shapes.push_back( new Triangle(H,G,C,red, someGloss));

    // LEFT
    shapes.push_back( new Triangle(G,E,C,red, someGloss));
    shapes.push_back( new Triangle(E,A,C,red, someGloss));

    // TOP
    shapes.push_back( new Triangle(G,F,E,red, superGloss));
    shapes.push_back( new Triangle(G,H,F,red, superGloss));

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
    shapes.push_back( new Triangle(E,B,A,blue, highGloss) );
    shapes.push_back( new Triangle(E,F,B,blue, highGloss) );

    // Front
    shapes.push_back( new Triangle(F,D,B,blue, highGloss) );
    shapes.push_back( new Triangle(F,H,D,blue, highGloss) );

    // BACK
    shapes.push_back( new Triangle(H,C,D,blue, highGloss) );
    shapes.push_back( new Triangle(H,G,C,blue, highGloss) );

    // LEFT
    shapes.push_back( new Triangle(G,E,C,blue, highGloss) );
    shapes.push_back( new Triangle(E,A,C,blue, highGloss) );

    // TOP
    shapes.push_back( new Triangle(G,F,E,blue, superGloss) );
    shapes.push_back( new Triangle(G,H,F,blue, superGloss) );

    // ---------------------------------------------------------------------------
    // Sphere
    // float radius = 100;
    // float empty = 0;
    // A = vec4(320, 300, 200, 1);
    // vec3 emptyv3 =  vec3(0,0,0);
    // vec3 ggg = vec3(1,1,1);
    // shapes.push_back( new Sphere( A, radius, blue, ggg, new Translucent(1, 0.6)));

}
#endif
