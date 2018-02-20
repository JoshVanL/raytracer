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
    glm::vec3 color;
    float power;
    const int max_bounces = 3;
    int bounces = 0;
    Ray(glm::vec4 pos, glm::vec4 dir, glm::vec3 col, float pow, const int bounces = 0) : 
     position(pos), direction(dir), color(col), power(pow), bounces(bounces){

    };
    Ray(){  position      = glm::vec4(0, -0.5, -1.4, 1.0);
            direction     = glm::vec4(0,0,1,1);
            color         = glm::vec3(1,1,1);
            power         = 10.f;
    };
    bool ClosestIntersection(const std::vector<Shape2D*>& shapes, Intersection &closestIntersection, const Shape2D* exclusion = nullptr) {
        closestIntersection.distance = std::numeric_limits<float>::max();
        bool found = false;

        glm::vec4 intersectionPoint;

        for (size_t i=0; i<shapes.size(); i++) {
            if((shapes[i]->intersect(*(this), (vec3) direction, intersectionPoint)) && (shapes[i] != exclusion)){

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

    vec3 GetDirectLight(Intersection& point, const vector<Shape2D*>& shapes) {

        if (IsOccluded(point, shapes)) {
            return vec3(0.0001, 0.0001, 0.0001);
        }

        float dist = glm::distance(position, point.position);

        vec3 surfaceNormal =  point.shape2D->getnormal(point.position, point.direction);
        vec3 lightToPoint =   glm::normalize((vec3) point.position - (vec3) position);

        float dotProduct = glm::dot(surfaceNormal, lightToPoint);
        float powPerSurface = (power * std::max(dotProduct, 0.f))/(4 * PI * pow(dist, 2));
        return color * powPerSurface;
    }
    bool IsOccluded(Intersection& point, const vector<Shape2D*>& shapes){
        Intersection intersect;
        vec4 dir = point.position - position;
        if(ClosestIntersection(dir, shapes, intersect, intersect.shape2D)){
            if(intersect.shape2D != point.shape2D)
                return true;
        }
        return false;
    }

    vec3 GetIndirectLight(){
        return vec3(0.3,0.2,0.18);
    }

};


#endif 