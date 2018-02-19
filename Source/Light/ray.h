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
            power         = 20.f;
    };
    bool ClosestIntersection(vec4 dir, std::vector<Shape2D*>& shapes, Intersection &closestIntersection) {
        closestIntersection.distance = std::numeric_limits<float>::max();
        bool found = false;

        glm::vec4 intersectionPoint;

        for (size_t i=0; i<shapes.size(); i++) {
            if(shapes[i]->intersect(*(this), (vec3) dir, intersectionPoint)){

                float d = glm::distance(intersectionPoint, position);

                if (d < closestIntersection.distance) {
                    closestIntersection.position = intersectionPoint;
                    closestIntersection.distance = d;
                    closestIntersection.shape2D = shapes[i];

                    found = true;
                }
                   
            }
        }
        return found;
    }

// bool ClosestIntersectionTriangles(vec4 dir, vector<Triangle> &triangles, Intersection &closestIntersection) {
    //     closestIntersection.distance = std::numeric_limits<float>::max();
    //     bool found = false;

    //     for (size_t i=0; i<triangles.size(); i++) {
    //         vec3 worldIntersectionCoordinates = GetIntersection(position, dir, triangles[i]);
    //         float t = worldIntersectionCoordinates.x;
    //         float u = worldIntersectionCoordinates.y;
    //         float v = worldIntersectionCoordinates.z; 

    //         if (0 <= t && 0 <= u && 0 <= v && u + v <= 1) { 
    //             vec4 camIntersectionCoordinates = triangles[i].tocamcoordinates(u,v);// WorldToCamCoordinates(u, v, triangles[i]);
    //             float d = glm::length(camIntersectionCoordinates - position);

    //             if (d < closestIntersection.distance) {
    //                 closestIntersection.position = camIntersectionCoordinates;
    //                 closestIntersection.distance = d;
    //                 closestIntersection.triangle = &(triangles[i]);

    //                 found = true;
    //             }
    //         }
    //     }
    //     return found;
    // }

    // vec3 GetIntersection(vec4 start, vec4 dir, Triangle& triangle){
    //     vec4 v0 = triangle.v0;
    //     vec4 v1 = triangle.v1;
    //     vec4 v2 = triangle.v2;
    //     vec3 e1 = vec3(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
    //     vec3 e2 = vec3(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);
    //     vec3 b = vec3(start.x - v0.x, start.y - v0.y, start.z - v0.z);
    //     vec3 d = (vec3) dir;
    //     mat3 A( -d, e1, e2 );
    //     vec3 x = glm::inverse( A ) * b;
    //     return x;
// }

    bool IsOccluded(Intersection& point, vector<Shape2D*>& shapes){
        glm::vec4 intersectionPoint;
        vec4 dir = position - point.position;
        for (int i=0; i<shapes.size(); i++) {
            if( !(*(shapes[i]) == *(point.shape2D)) ) {
                if(shapes[i]->intersect(*(this), (vec3) dir, intersectionPoint)){
                    float d = glm::distance(intersectionPoint, position);
                    if (d < point.distance) {
                        return true;
                    }
                } 
            }
        }
        return false;
    }

    vec3 GetIndirectLight(){
        return vec3(0.3,0.2,0.18);
    }

    vec3 GetDirectLight(Intersection& point, vector<Shape2D*>& shapes) {

        /* If occluded, return a shadow */
        if (IsOccluded(point, shapes)) {
            return vec3(0.0001, 0.0001, 0.0001);
        }

        /* Otherwise scale the point's color by the light intensity hitting is */
        float dist = glm::distance(position, point.position);

        vec3 surfaceNormal =  point.shape2D->getnormal();
        vec3 lightToPoint =   glm::normalize((vec3) point.position - (vec3) position);

        float dotProduct = glm::dot(surfaceNormal, lightToPoint);
        float powPerSurface = (power * std::max(dotProduct, 0.f))/(4 * PI * pow(dist, 2));
        return point.shape2D->color * color * powPerSurface;
    }
};


#endif 