
#ifndef INTERSECTION_H
#define INTERSECTION_H
#include <vector>
#include <glm/glm.hpp>
#include "../shapes/shape2D.h"
#include "../materials/material_properties/gloss.h"



class Intersection {
public:
    glm::vec4 position;
    glm::vec4 direction;
    glm::vec2 uv;
    std::vector<Material*> materials;
    float distance;
    int triangleIndex;
    glm::vec3 lightAtIntersection;
    Shape2D* shape2D;

    Intersection(   glm::vec4 pos, 
                    glm::vec4 dir, 
                    float dis, 
                    Shape2D* shape2D): 
                    position(pos), direction(dir), distance(dis), shape2D(shape2D){

    };
    Intersection(){

    };
    glm::vec3 compute_color(const Ray& primary_ray, const std::vector<Shape2D*>& shapes, LightSource* lightSource);
};


#endif
