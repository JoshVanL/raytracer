#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>
#include "../Shapes/shape2D.h"
#include "../Shapes/triangle.h"
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
    glm::vec3 color;
    float power;
    Ray(vec4 pos, vec3 col, float pow):  position(pos), color(col), power(pow){};
    Ray(){  position      = glm::vec4(0, -0.5, -1.4, 1.0);
            color         = glm::vec3(1,1,1);
            power         = 20.f;
    };

    bool ClosestIntersectionTriangles(vec4 dir, vector<Triangle> &triangles, Intersection &closestIntersection) {
        closestIntersection.distance = std::numeric_limits<float>::max();
        bool found = false;

        for (size_t i=0; i<triangles.size(); i++) {
            vec3 worldIntersectionCoordinates = GetIntersection(position, dir, triangles[i]);
            float t = worldIntersectionCoordinates.x;
            float u = worldIntersectionCoordinates.y;
            float v = worldIntersectionCoordinates.z; 

            if (0 <= t && 0 <= u && 0 <= v && u + v <= 1) { 
                vec4 camIntersectionCoordinates =  WorldToCamCoordinates(u, v, triangles[i]);
                float d = glm::length(camIntersectionCoordinates - position);

                if (d < closestIntersection.distance) {
                    closestIntersection.position = camIntersectionCoordinates;
                    closestIntersection.distance = d;
                    closestIntersection.triangle = &(triangles[i]);

                    found = true;
                }
            }
        }
        return found;
    }

    vec3 GetIntersection(vec4 start, vec4 dir, Triangle& triangle){
        vec4 v0 = triangle.v0;
        vec4 v1 = triangle.v1;
        vec4 v2 = triangle.v2;
        vec3 e1 = vec3(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
        vec3 e2 = vec3(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);
        vec3 b = vec3(start.x - v0.x, start.y - v0.y, start.z - v0.z);
        vec3 d = (vec3) dir;
        mat3 A( -d, e1, e2 );
        vec3 x = glm::inverse( A ) * b;
        return x;
    }

    bool IsOccludedTriangles(Intersection point, vector<Triangle>& triangles){

        vec4 t_p = point.triangle->v0;
        vec4 u_p = point.triangle->v1;
        vec4 v_p = point.triangle->v2;
        vec4 dir = position - point.position;
        for (int i=0; i<triangles.size(); i++) {
            vec3 intersect = GetIntersection(point.position, dir, triangles[i]);

            float t = intersect.x;
            float u = intersect.y;
            float v = intersect.z;

            if(0 < t && 0 <= u && 0 <= v && u + v <= 1 &&
                    (t_p != triangles[i].v0 ||
                    u_p != triangles[i].v1 ||
                    v_p != triangles[i].v2))
                return true;
        }
        return false;
    }

    vec3 GetIndirectLightAt(){
        return vec3(0.3,0.2,0.18);
    }

    vec3 GetDirectLightAtTriangles(Intersection& point, vector<Triangle>& triangles) {

        /* If occluded, return a shadow */
        if (IsOccludedTriangles(point, triangles)) {
            return vec3(0.0001, 0.0001, 0.0001);
        }

        /* Otherwise scale the point's color by the light intensity hitting is */
        float dist = glm::distance(position, point.position);

        vec3 surfaceNormal =  glm::normalize(triangleNormal((vec3) point.triangle->v0, (vec3) point.triangle->v1, (vec3) point.triangle->v2));
        vec3 lightToPoint = glm::normalize((vec3) point.position - (vec3) position);

        float dotProduct = dot(surfaceNormal, lightToPoint);
        float powPerSurface = (power * std::max(dotProduct, 0.f))/(4 * PI * pow(dist, 2));
        return point.triangle->color * color * powPerSurface;
    }

    static vec4 WorldToCamCoordinates(float u, float v, Triangle& tri) {
        vec4 e1 = tri.v1 - tri.v0;
        vec4 e2 = tri.v2 - tri.v0;
        return tri.v0 + u * e1 + v * e2;
    }


};


#endif 