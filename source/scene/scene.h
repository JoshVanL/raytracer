#ifndef SCENE_H
#define SCENE_H

// Defines a simple test model: The Cornel Box

#include <vector>

#include <glm/glm.hpp>

#include "../shapes/triangle.h"
#include "../shapes/sphere.h"
#include "../materials/texture.h"

#include "model.h"
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
    vec3 blue(   0.10f, 0.10f, 0.70f );
    vec3 purple( 0.75f, 0.15f, 0.75f );
    vec3 white( 0.75f, 0.75f, 0.75f );

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
    shapes.push_back( new Triangle( C, B, A, green, new Texture("source/materials/textures/carpet.bmp") ) );
    shapes.push_back( new Triangle( C, D, B, green, new Texture("source/materials/textures/carpet.bmp") ) );

    //// Left wall
    shapes.push_back( new Triangle( A, E, C, purple, new Texture("source/materials/textures/wall1.bmp") ) );
    shapes.push_back( new Triangle( C, E, G, purple, new Texture("source/materials/textures/wall1.bmp") ) );

    // Right wall
    shapes.push_back( new Triangle( F, B, D, yellow, new Texture("source/materials/textures/wall2.bmp") ) );
    shapes.push_back( new Triangle( H, F, D, yellow, new Texture("source/materials/textures/wall2.bmp") ) );

    // Ceiling
    shapes.push_back( new Triangle( E, F, G, cyan, new Texture("source/materials/textures/ceil.bmp") ) );
    shapes.push_back( new Triangle( F, H, G, cyan, new Texture("source/materials/textures/ceil.bmp") ) );

    // Back wall
    shapes.push_back( new Triangle( G, D, C, white, new Texture("source/materials/textures/water.bmp") ) );
    shapes.push_back( new Triangle( G, H, D, white, new Texture("source/materials/textures/water.bmp") ) );

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



    // FRONT
    shapes.push_back( new Triangle(E,B,A,red, new Texture("source/materials/textures/wood.bmp")));
    shapes.push_back( new Triangle(E,F,B,red, new Texture("source/materials/textures/wood.bmp")));
    shapes.push_back( new Triangle(F,D,B,red, new Texture("source/materials/textures/wood.bmp")));
    shapes.push_back( new Triangle(F,H,D,red, new Texture("source/materials/textures/wood.bmp")));

    // BACK
    shapes.push_back( new Triangle(H,C,D,red, new Texture("source/materials/textures/wood.bmp")));
    shapes.push_back( new Triangle(H,G,C,red, new Texture("source/materials/textures/wood.bmp")));

    // LEFT
    shapes.push_back( new Triangle(G,E,C,red, new Texture("source/materials/textures/wood.bmp")));
    shapes.push_back( new Triangle(E,A,C,red, new Texture("source/materials/textures/wood.bmp")));

    // TOP
    shapes.push_back( new Triangle(G,F,E,red, new Texture("source/materials/textures/wood.bmp")));
    shapes.push_back( new Triangle(G,H,F,red, new Texture("source/materials/textures/wood.bmp")));

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
    //shapes.push_back( new Triangle(E,B,A,blue, new Texture("source/materials/textures/wood.bmp")));
    //shapes.push_back( new Triangle(E,F,B,blue, new Texture("source/materials/textures/wood.bmp")));

    //// Front
    //shapes.push_back( new Triangle(F,D,B,blue, new Texture("source/materials/textures/wood.bmp")));
    //shapes.push_back( new Triangle(F,H,D,blue, new Texture("source/materials/textures/wood.bmp")));

    //// BACK
    //shapes.push_back( new Triangle(H,C,D,blue, new Texture("source/materials/textures/wood.bmp")));
    //shapes.push_back( new Triangle(H,G,C,blue, new Texture("source/materials/textures/wood.bmp")));

    //// LEFT
    //shapes.push_back( new Triangle(G,E,C,blue, new Texture("source/materials/textures/wood.bmp")));
    //shapes.push_back( new Triangle(E,A,C,blue, new Texture("source/materials/textures/wood.bmp")));

    //// TOP
    //shapes.push_back( new Triangle(G,F,E,blue, new Texture("source/materials/textures/wood.bmp")));
    //shapes.push_back( new Triangle(G,H,F,blue, new Texture("source/materials/textures/wood.bmp")));

    // ---------------------------------------------------------------------------
    // Sphere
    float radius = 100;
    float empty = 0;
    A = vec4(320, 300, 200, 1);
    vec3 emptyv3 =  vec3(0,0,0);
    vec3 ggg = vec3(1,1,1);
    //shapes.push_back( new Sphere( A, radius, blue, ggg, new Reflective()));

    // ---------------------------------------------------------------------------
    //

    vector<Shape2D*> chair = uploadModel("source/assets/chair.obj", "source/assets/red.bmp", vec4(300, 0, 20, 1), vec4(500, 180, 200, 1), vec3(0.5, 0.5, 0.5));
    for (int i = 0; i < chair.size(); i++) {
        shapes.push_back(chair[i]);
    }

    vector<Shape2D*> teapot = uploadModel("source/assets/teapot.obj", "source/assets/wood.bmp", vec4(100, 180, 20, 1), vec4(300, 280, 200, 1), vec3(0.7, 0.7, 0.5));
    for (int i = 0; i < teapot.size(); i++) {
        shapes.push_back(teapot[i]);
    }

    vector<Shape2D*> plane = uploadModel("source/assets/plane.obj", "source/assets/green.bmp", vec4(100, 350, 220, 1), vec4(300, 450, 400, 1), vec3(0.5, 0.4, 0.8));
    for (int i = 0; i < plane.size(); i++) {
        shapes.push_back(plane[i]);
    }

    vector<Shape2D*> sky = uploadModel("source/assets/skyscraper.obj", "source/assets/metal.bmp", vec4(100, 0, 300, 1), vec4(600, 500, 400, 1), vec3(1, 1, 1));
    for (int i = 0; i < sky.size(); i++) {
        shapes.push_back(sky[i]);
    }

}
#endif
