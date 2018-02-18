#ifndef CAMERA_H
#define CAMERA_H
#include "SDL.h"
#include <glm/glm.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat4;

class Camera {
public:
    const int SCREEN_WIDTH = 320;
    const int SCREEN_HEIGHT = 256;
    const bool FULLSCREEN_MODE = false;
    const vec3 INDIRECT_LIGHT  = vec3(0.3,0.2,0.18);
    const float ANG = 0.1;
    const mat4 ROTATE_RIGHT =   mat4(vec4(cos(-ANG), 0, -sin(-ANG), 0),  vec4(0, 1, 0, 0),               vec4(sin(-ANG), 0, cos(-ANG), 0),    vec4(0,0,0,1));
    const mat4 ROTATE_LEFT  =   mat4(vec4(cos(ANG), 0, -sin(ANG), 0),    vec4(0, 1, 0, 0),               vec4(sin(ANG), 0,  cos(ANG), 0),      vec4(0,0,0,1));
    const mat4 ROTATE_UP    =   mat4(vec4(1, 0, 0, 0),                   vec4(0, cos(ANG), sin(ANG), 0),     vec4(0, -sin(ANG), cos(ANG), 0),     vec4(0,0,0,1));
    const mat4 ROTATE_DOWN  =   mat4(vec4(1, 0, 0, 0),                   vec4(0, cos(-ANG), sin(-ANG), 0),   vec4(0, -sin(-ANG), cos(-ANG), 0),   vec4(0,0,0,1));

    vec4 position;
    mat4 rotation;
    float yaw;
    float focalLength;
    Camera(vec4 pos, float foc, const mat4& rot = mat4(vec4(1,0,0,1), vec4(0,1,0,1), vec4(0,0,1,1), vec4(0,0,0,1)))
    :   position(pos), focalLength(foc), rotation(rot)
    {

    }
    
    Camera()
    {
        rotation           = mat4(vec4(1,0,0,1), vec4(0,1,0,1), vec4(0,0,1,1), vec4(0,0,0,1));
        position           = vec4(0, 0, -2.25, 1);
        focalLength        = SCREEN_WIDTH/2;
    }
    
    void translateCamera(SDL_KeyboardEvent key){
        switch( key.keysym.sym ){
            case SDLK_UP:
                position += rotation*vec4(0,0,1,0);
                break;
            case SDLK_LEFT:
                position += rotation*vec4(-0.5,0,0,0);
                break;
            case SDLK_DOWN:
                position += rotation*vec4(0,0,-1,0);
                break;
            case SDLK_RIGHT :
                position += rotation*(vec4(0.5,0,0,0));
                break;
        }
    }

    void rotateCamera(SDL_KeyboardEvent key){
        switch( key.keysym.sym ){
            case SDLK_UP:
                rotation = rotation*ROTATE_UP;
                break;
            case SDLK_LEFT:
                rotation = rotation*ROTATE_LEFT;
                break;
            case SDLK_DOWN:
                rotation = rotation*ROTATE_DOWN;
                break;
            case SDLK_RIGHT :
                rotation = rotation*ROTATE_RIGHT;
                break;
        }
    }
};



#endif