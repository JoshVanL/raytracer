#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>
#include "intersection.h"
#include <vector>

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
    const int max_bounces = 3;
    int bounces = 0;
    Ray(glm::vec4 pos, glm::vec4 dir, const int bounces = 0) : 
     position(pos), direction(dir), bounces(bounces){

    };
    Ray(){  position      = glm::vec4(0, -0.5, -1.4, 1.0);
            direction     = glm::vec4(0,0,1,1);
    };
    bool ClosestIntersection(const std::vector<Shape2D*>& shapes, Intersection &closestIntersection,
                             const Shape2D* exclusion = nullptr) {
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

};


#endif 