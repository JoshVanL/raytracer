#ifndef WAVE_H
#define WAVE_H 
#include "shape3D.h"
#include <glm/glm.hpp>
#include "shape2D.h"
#include "../light/intersection.h"
#include <glm/gtx/perpendicular.hpp>
using glm::vec2;
using glm::vec3;
using glm::vec4;

//////////////////////////////////////// 
// UNUSED
////////////////////////////////////////

// class Wave : public Shape2D {
// public:

//     vec4 topright;
//     vec4 botleft;

//     Wave(vec4 topright, vec4 bottomleft, glm::vec3 surfaceColor, glm::vec3 emissionColor, float transparency, float reflection): Shape2D()
//     {
    
//     };
    
//     virtual bool intersect(const glm::vec3 &rayorig, const glm::vec3 &raydir, float &t0, float &t1) const {
//         vec3 dir = raydir;
//         vec3 a =  (vec3) glm::normalize(glm::perp(vec4(botleft.x, topright.y, topright.z, 1) - topright, vec4(topright.x, botleft.y, botleft.z, 1) - botleft) );
//         vec3 b = -a;
//         vec3 norm;
//         if(glm::dot(a, (vec3) glm::normalize(raydir)) <= 0){
//             norm = a;
//         }
//         else{
//             norm = b;
//         }
//         float denom = glm::dot(norm, dir);
//         vec3 center = vec3( botleft.x + (topright.x - botleft.x)/2, botleft.y + (topright.y - botleft.y)/2, botleft.z - (topright.z - botleft.z)/2 );
//         if (denom > 1e-6) { 
//             vec3 p0l0 = center - rayorig; 
//             float t = glm::dot(p0l0, norm) / denom; 
//             return (t >= 0); 
//         }
//         return false; 
//     }

//     glm::vec3 trace(const glm::vec3 &rayorig, const glm::vec3 &raydir,  const std::vector<Cuboid> &cuboids, const int &depth){

//     }
//     virtual glm::vec3 getnormal(Intersection& intersection){
//         vec3 a =  (vec3) glm::normalize(glm::perp(vec4(botleft.x, topright.y, topright.z, 1) - topright, vec4(topright.x, botleft.y, botleft.z, 1) - botleft) );
//         vec3 b = -a;
//         vec2 txyx = vec2(1 ,cos(intersection.position.x))/(float)sqrt(1 + std::pow(cos(intersection.position.x),2))

        
//     }
    
// };



#endif 