#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>
#include "intersection.h"
#include <vector>
#include "../materials/material.h"
#include <algorithm>
#define PI           3.14159265358979323846
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;
using namespace std;

class Ray {

public:
    glm::vec4 position;
    glm::vec4 direction;
    const int max_depth = 3;
    int bounces = 0;

    Ray(    glm::vec4 pos,
            glm::vec4 dir,
            const int bounces = 0) :
     position(pos), direction(dir), bounces(bounces){

    };

    Ray(){  position      = glm::vec4(0, -0.5, -1.4, 1.0);
            direction     = glm::vec4(0,0,1,1);
    };

    bool ClosestIntersection(const std::vector<Shape2D*>& shapes, Intersection &closestIntersection,
                             const Shape2D* exclusion = nullptr) {
        if(bounces >= 3)
            return false;
        closestIntersection.distance = std::numeric_limits<float>::max();
        bool found = false;

        glm::vec4 intersectionPoint;
        for (size_t i=0; i<shapes.size(); i++) {
 
            if((shapes[i]->intersect(*(this), (vec3) direction,
                intersectionPoint)) && (shapes[i] != exclusion)){
  
                float d = glm::distance(intersectionPoint, position);

                if (d < closestIntersection.distance) {
                    closestIntersection.position = intersectionPoint;
                    closestIntersection.distance = d;
                    closestIntersection.direction = direction;
                    closestIntersection.shape2D = shapes[i];
                    found = true;
                }
            }
        }
        return found;
    }
    struct find_material
    {
        std::string s;
        find_material(std::string s) : s(s) {}
        bool operator()(const Material* t) const
        {
            return t->material_type == s;
        }
    };
    bool ClosestIntersection(const std::vector<Shape2D*>& shapes, Intersection &closestIntersection,
                             std::string material_type, const Shape2D* exclusion = nullptr) {
        closestIntersection.distance = std::numeric_limits<float>::max();
        bool found = false;

        glm::vec4 intersectionPoint;

        for (size_t i=0; i<shapes.size(); i++) {
            if((shapes[i]->intersect(*(this), (vec3) direction,
                intersectionPoint)) && (shapes[i] != exclusion)){

                float d = glm::distance(intersectionPoint, position);
                std::vector<Material*>::iterator it = std::find_if(shapes[i]->materials.begin(), shapes[i]->materials.end(), find_material(material_type));
                if(it != shapes[i]->materials.end()){
                    continue;
                }
                if (d < closestIntersection.distance) {
                    closestIntersection.position = intersectionPoint;
                    closestIntersection.distance = d;
                    closestIntersection.direction = direction;
                    closestIntersection.shape2D = shapes[i];

                    found = true;
                }
            }
        }
        return found;
    }
    Ray offset(vec4 offset_dir, float offset_scalar) {
        return Ray(position+ offset_scalar * offset_dir, direction, bounces);
    }
    bool ClosestIntersection(const std::string exclusion_id, const std::vector<Shape2D*>& shapes, Intersection &closestIntersection) 
    {
        closestIntersection.distance = std::numeric_limits<float>::max();
        bool found = false;

        glm::vec4 intersectionPoint;

        for (size_t i=0; i<shapes.size(); i++) {
            if((shapes[i]->intersect(*(this), (vec3) direction,
                intersectionPoint)) && (shapes[i]->id != exclusion_id)){

                float d = glm::distance(intersectionPoint, position);

                if (d < closestIntersection.distance) {
                    closestIntersection.position = intersectionPoint;
                    closestIntersection.distance = d;
                    closestIntersection.direction = direction;
                    closestIntersection.shape2D = shapes[i];

                    found = true;
                }
            }
        }
        return found;
    }

};


#endif
