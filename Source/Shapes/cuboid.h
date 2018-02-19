#ifndef CUBOID_H
#define CUBOID_H 
#include "shape3D.h"

using glm::vec4;

//////////////////////////////////////// 
// UNUSED
////////////////////////////////////////

class Cuboid : public Shape3D {
public:

    std::vector<Triangle> triangles;

    Cuboid(std::vector<Triangle> triangles, glm::vec3 center, glm::vec3 surfaceColor, glm::vec3 emissionColor, float transparency, float reflection):
        Shape3D(center, surfaceColor, emissionColor, transparency, reflection)
    {
    
    };

    Cuboid(glm::vec3 position, glm::vec3 surfaceColor, glm::vec3 emissionColor, float transparency, float reflection) :
        Shape3D(center, surfaceColor, emissionColor, transparency, reflection) 
    {
        vec4 A, B, C, D, E, F, G, H;
        
        A = vec4(290,0,114,1);
        B = vec4(130,0, 65,1);
        C = vec4(240,0,272,1);
        D = vec4( 82,0,225,1);

        E = vec4(290,165,114,1);
        F = vec4(130,165, 65,1);
        G = vec4(240,165,272,1);
        H = vec4( 82,165,225,1);

        // Front
        triangles.push_back( Triangle(E,B,A,surfaceColor, SOMEGLOSS) );
        triangles.push_back( Triangle(E,F,B,surfaceColor, SOMEGLOSS) );

        // Front
        triangles.push_back( Triangle(F,D,B,surfaceColor, SOMEGLOSS) );
        triangles.push_back( Triangle(F,H,D,surfaceColor, SOMEGLOSS) );

        // BACK
        triangles.push_back( Triangle(H,C,D,surfaceColor, SOMEGLOSS) );
        triangles.push_back( Triangle(H,G,C,surfaceColor, SOMEGLOSS) );

        // LEFT
        triangles.push_back( Triangle(G,E,C,surfaceColor, SOMEGLOSS) );
        triangles.push_back( Triangle(E,A,C,surfaceColor, SOMEGLOSS) );

        // TOP
        triangles.push_back( Triangle(G,F,E,surfaceColor, SUPERGLOSS) );
        triangles.push_back( Triangle(G,H,F,surfaceColor, SUPERGLOSS) );
    }


    virtual bool intersect(const glm::vec3 &rayorig, const glm::vec3 &raydir, float &t0, float &t1) const {
        
    }

    glm::vec3 trace(const glm::vec3 &rayorig, const glm::vec3 &raydir,  const std::vector<Cuboid> &cuboids, const int &depth){

    }
};



#endif 