#ifndef CAMERA_H
#define CAMERA_H
#include "SDL.h"

#include "../Light/ray.h"
#include <glm/glm.hpp>
#include <glm/gtx/normal.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat4;


class Camera {
public:

    /////////////////////// GLOBAL VARS //////////////////////////////
    const int SCREEN_WIDTH = 320;
    const int SCREEN_HEIGHT = 256;
    const bool FULLSCREEN_MODE = false;
    const vec3 INDIRECT_LIGHT  = vec3(0.3,0.2,0.18);
    const float ANG = 0.1;
    const mat4 ROTATE_LEFT =   mat4(vec4(cos(-ANG*2), 0, -sin(-ANG*2), 0),  vec4(0, 1, 0, 0),               vec4(sin(-ANG*2), 0, cos(-ANG*2), 0),    vec4(0,0,0,1));
    const mat4 ROTATE_RIGHT  =   mat4(vec4(cos(ANG*2), 0, -sin(ANG*2), 0),    vec4(0, 1, 0, 0),               vec4(sin(ANG*2), 0,  cos(ANG*2), 0),      vec4(0,0,0,1));
    const mat4 ROTATE_UP    =   mat4(vec4(1, 0, 0, 0),                   vec4(0, cos(ANG), sin(ANG), 0),     vec4(0, -sin(ANG), cos(ANG), 0),     vec4(0,0,0,1));
    const mat4 ROTATE_DOWN  =   mat4(vec4(1, 0, 0, 0),                   vec4(0, cos(-ANG), sin(-ANG), 0),   vec4(0, -sin(-ANG), cos(-ANG), 0),   vec4(0,0,0,1));
    //////////////////////////////////////////////////////////////////


    vec4 position;
    mat4 rotation;
    float yaw;
    float focal_length;
    Ray primary_ray;

    Camera(vec4 pos, float foc, const mat4& rot = mat4(vec4(1,0,0,1), vec4(0,1,0,1), vec4(0,0,1,1), vec4(0,0,0,1)))
    :   position(pos), focal_length(foc), rotation(rot), primary_ray(pos, vec4(0,0,1,1), vec3(0,0,0), 0)
    {
    }
    
    Camera()
    {
        rotation           = mat4(vec4(1,0,0,1), vec4(0,1,0,1), vec4(0,0,1,1), vec4(0,0,0,1));
        position           = vec4(0, 0, 0, 1);
        focal_length       = SCREEN_WIDTH/2;
        primary_ray.color = vec3(1,1,1);
        primary_ray.position = position;
        primary_ray.power = 0;
    }

    vec4 getDirection(int i, int j){
        return glm::normalize(rotation * vec4(  i - SCREEN_WIDTH/2 - position.x,
                                                j - SCREEN_HEIGHT/2 - position.y,
                                                focal_length - position.z,
                                                1));
    }

    void translateCameraVert(SDL_KeyboardEvent key){
        switch( key.keysym.sym ){
            case SDLK_DOWN:
                primary_ray.position += rotation*vec4(0,0.3f,0,0);
                position += rotation*vec4(0,0.3f,0,0);
                break;
            case SDLK_LEFT:
                primary_ray.position += rotation*vec4(-0.5,0,0,0);
                position += rotation*vec4(-0.5f,0,0,0);
                break;
            case SDLK_UP:
                primary_ray.position += rotation*vec4(0,-0.3f,0,0);
                position += rotation*vec4(0,0,-0.3f,0);
                break;
            case SDLK_RIGHT :
                primary_ray.position += rotation*vec4(0.5f,0,0,0);
                position += rotation*(vec4(0.5f,0,0,0));
                break;
        }
    }

    void translateCamera(SDL_KeyboardEvent key){
        switch( key.keysym.sym ){
            case SDLK_UP:
                primary_ray.position += rotation*vec4(0,0,0.3f,0);
                position += rotation*vec4(0,0,0.3f,0);
                break;
            case SDLK_LEFT:
                primary_ray.position += rotation*vec4(-0.5f,0,0,0);
                position += rotation*vec4(-0.5f,0,0,0);
                break;
            case SDLK_DOWN:
                primary_ray.position += rotation*vec4(0,0,-0.3f,0);
                position += rotation*vec4(0,0,-0.3f,0);
                break;
            case SDLK_RIGHT :
                primary_ray.position += rotation*vec4(0.5f,0,0,0);
                position += rotation*(vec4(0.5f,0,0,0));
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

    Ray createNewRay(int i, int j) {
        float camera_x = i - SCREEN_WIDTH / 2;
        float camera_y = j - SCREEN_HEIGHT / 2;

        vec4 dir = vec4((i - SCREEN_WIDTH / 2) - position.x, 
                        (j - SCREEN_HEIGHT/ 2) - position.y, 
                        focal_length - position.z, 1);

        return Ray(position, dir, vec3(1,1,1), 1);
    }

};



#endif