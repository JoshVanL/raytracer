#ifndef RASTERISER_H
#define RASTERISER_H
#include <glm/glm.hpp>
#include <glm/gtx/normal.hpp>
#include "../light/lightsource.h"
#include <math.h>
using glm::vec3;
using glm::vec4;
class Rasteriser {
public:

    Rasteriser(){};

    static glm::mat4 ViewMatrix(vec3 eye, vec3 center, vec3 up) {
        vec3 zaxis = glm::normalize(eye - center);
        vec3 xaxis = glm::normalize(glm::cross(up, zaxis));
        vec3 yaxis = glm::cross(zaxis, xaxis);

        glm::mat4 orientation(  vec4( xaxis.x, yaxis.x, zaxis.x, 0 ),
                                vec4( xaxis.y, yaxis.y, zaxis.y, 0 ),
                                vec4( xaxis.z, yaxis.z, zaxis.z, 0 ),
                                vec4(   0,       0,       0,     1 ));

        glm::mat4 translation(
                                vec4(   1,      0,      0,   0 ),
                                vec4(   0,      1,      0,   0 ), 
                                vec4(   0,      0,      1,   0 ),
                                vec4(-eye.x, -eye.y, -eye.z, 1 )
                        );            
        return ( orientation * translation );          
    }



    static glm::mat4 ViewPortMatrix(int x, int y, int w, int h) {

        glm::mat4 viewport(  vec4( w/2.f,     0,       0,     (y + ( y + h))/2.f ),
                             vec4(   0,       w/2.f,   0.5,   (x + ( x + w))/2.f ),
                             vec4(   0,       0,       0.5,   0.5f ),
                             vec4(   0,       0,       0,     1    ));
      
        return viewport;          
    }   

    static glm::mat4 PerspectiveProjectionMatrix(float fov, int y, int w, int h) {
        float s = 1.f/(tan(fov/2.f) * (PI/180.f));

        glm::mat4 projection(  vec4(   s,       0,       0,      0 ),
                               vec4(   0,       s,       0,      0 ),
                               vec4(   0,       0,       -1.f,  -1 ),
                               vec4(   0,       0,       0,      0 ));
      
        return projection;          
    }  

};


#endif 