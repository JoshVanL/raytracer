#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>
#include "../Shapes/shape2D.h"
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
    glm::vec3 color;
    float power;
    Ray(vec4 pos, vec3 col, float pow):  position(pos), color(col), power(pow){};
    Ray(){  position      = glm::vec4(0, -0.5, -1.4, 1.0);
            color         = glm::vec3(1,1,1);
            power         = 10.f;
    };
    bool ClosestIntersection(vec4 dir, std::vector<Shape2D*>& shapes, Intersection &closestIntersection, const Shape2D* exclusion = nullptr) {
        closestIntersection.distance = std::numeric_limits<float>::max();
        bool found = false;

        glm::vec4 intersectionPoint;

        for (size_t i=0; i<shapes.size(); i++) {
            if((shapes[i]->intersect(*(this), (vec3) dir, intersectionPoint)) && (shapes[i] != exclusion)){

                float d = glm::distance(intersectionPoint, position);

                if (d < closestIntersection.distance) {
                    closestIntersection.position = intersectionPoint;
                    closestIntersection.distance = d;
                    closestIntersection.direction = dir;
                    closestIntersection.shape2D = shapes[i];

                    found = true;
                }
                   
            }
        }
        return found;
    }

    vec3 GetDirectLight(Intersection& point, vector<Shape2D*>& shapes) {

        if (IsOccluded(point, shapes)) {
            return vec3(0.0001, 0.0001, 0.0001);
        }

        float dist = glm::distance(position, point.position);

        vec3 surfaceNormal =  point.shape2D->getnormal(point.position, point.direction);
        vec3 lightToPoint =   glm::normalize((vec3) point.position - (vec3) position);

        float dotProduct = glm::dot(surfaceNormal, lightToPoint);
        float powPerSurface = (power * std::max(dotProduct, 0.f))/(4 * PI * pow(dist, 2));
        return point.shape2D->color * color * powPerSurface;
    }
    bool IsOccluded(Intersection& point, vector<Shape2D*>& shapes){
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