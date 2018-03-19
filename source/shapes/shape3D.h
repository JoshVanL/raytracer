#ifndef SHAPE_H
#define SHAPE_H

#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/normal.hpp>
#include "shape2D.h"

using glm::vec3;
using glm::vec4;
#define INFINITY_ 1e8 
#define RED             vec3 (0.75f, 0.15f, 0.15f )
#define YELLOW          vec3(0.75f, 0.75f, 0.15f )
#define GREEN           vec3(0.15f, 0.75f, 0.15f )
#define CYAN            vec3(0.15f, 0.75f, 0.75f )
#define BLUE            vec3(0.15f, 0.15f, 0.75f )
#define PURPLE          vec3(0.75f, 0.15f, 0.75f )
#define WHITE           vec3(0.75f, 0.75f, 0.75f )
#define LOWGLOSS        vec3(0.7, 0.7, 0.7)
#define NORMALGLOSS     vec3(1, 1, 1)
#define SOMEGLOSS       vec3(1.2, 1.2, 1.2)
#define MOREGLOSS       vec3(1.5, 1.5, 1.5)
#define HIGHGLOSS       vec3(2, 2, 2)
#define SUPERGLOSS      vec3(5, 5, 5)

class Shape3D {
public:
  
    const std::vector<Shape2D*> faces;
    const std::string identifier;
    Shape3D(std::vector<Shape2D*>& faces, std::string id): faces(faces), identifier(id){

    };


    static float mix(const float &a, const float &b, const float &mix) 
    { 
        return b * mix + a * (1 - mix); 
    };

    virtual ~Shape3D() = 0;
};

Shape3D::~Shape3D(){}

#endif