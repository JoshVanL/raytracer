#ifndef CAMERA_H
#define CAMERA_H
#include "SDL.h"

#include "../light/ray.h"
#include <glm/glm.hpp>
#include <glm/gtx/normal.hpp>
#include "fisheye.h"
using glm::vec3;
using glm::vec4;
using glm::mat4;

enum CameraEffectType{
    FISHEYE,
    NONE
};

class Camera {
public:
 
    /////////////////////// GLOBAL VARS //////////////////////////////
    const int SCREEN_WIDTH = 1800;
    const int SCREEN_HEIGHT = 1800;
    const bool FULLSCREEN_MODE = false;
    const vec3 INDIRECT_LIGHT  = vec3(0.3,0.2,0.18);
    const float ANG = 0.05;
    const mat4 ROTATE_LEFT =   mat4(vec4(cos(-ANG*2), 0, -sin(-ANG*2), 0),  vec4(0, 1, 0, 0),               vec4(sin(-ANG*2), 0, cos(-ANG*2), 0),    vec4(0,0,0,1));
    const mat4 ROTATE_RIGHT  =   mat4(vec4(cos(ANG*2), 0, -sin(ANG*2), 0),    vec4(0, 1, 0, 0),               vec4(sin(ANG*2), 0,  cos(ANG*2), 0),      vec4(0,0,0,1));
    const mat4 ROTATE_UP    =   mat4(vec4(1, 0, 0, 0),                   vec4(0, cos(ANG), sin(ANG), 0),     vec4(0, -sin(ANG), cos(ANG), 0),     vec4(0,0,0,1));
    const mat4 ROTATE_DOWN  =   mat4(vec4(1, 0, 0, 0),                   vec4(0, cos(-ANG), sin(-ANG), 0),   vec4(0, -sin(-ANG), cos(-ANG), 0),   vec4(0,0,0,1));
    //////////////////////////////////////////////////////////////////


    vec4 position;
    mat4 rotation;
    float focal_length;
    Ray primary_ray;
    CameraEffect* currentEffect;
    Fisheye fisheye;

    Camera(     vec4 pos, 
                float foc, 
                CameraEffectType cameff = CameraEffectType::NONE,
                const mat4& rot = mat4(vec4(1,0,0,1), vec4(0,1,0,1), vec4(0,0,1,1), vec4(0,0,0,1)))
                : position(pos), focal_length(foc), rotation(rot), primary_ray(createNewRay((int)SCREEN_WIDTH/2, (int)SCREEN_HEIGHT/2)),
                  currentEffect(get_camera_effect(cameff)), fisheye(Fisheye(1.0, (double)SCREEN_WIDTH, (double)SCREEN_HEIGHT))
    {
    }
    
    Camera() :  position(vec4(0, -2.25, 0, 1)), rotation(mat4(vec4(1,0,0,1), vec4(0,1,0,1), vec4(0,0,1,1), vec4(0,0,0,1))), focal_length(SCREEN_WIDTH/2), 
                currentEffect(get_camera_effect(CameraEffectType::NONE)),
                primary_ray(createNewRay((int)SCREEN_WIDTH/2, (int)SCREEN_HEIGHT/2) ), 
                fisheye(Fisheye(1.0, (double)SCREEN_WIDTH, (double)SCREEN_HEIGHT))
    {
    }

    vec4 getDirection(int i, int j){
        return glm::normalize(rotation * vec4(  i - SCREEN_WIDTH/2 - position.x,
                                                j - SCREEN_HEIGHT/2 - position.y,
                                                focal_length - position.z,
                                                1));
    }

    CameraEffect* get_camera_effect(CameraEffectType camefftype){
        if(camefftype == CameraEffectType::NONE)
            return nullptr;
        if(camefftype == CameraEffectType::FISHEYE)
            return &fisheye;
    }

    void translateCameraVert(SDL_KeyboardEvent key){
        switch( key.keysym.sym ){
            case SDLK_DOWN:
                primary_ray.position += rotation*vec4(0,0.1f,0,0);
                position += rotation*vec4(0,0.1f,0,0);
                break;
            case SDLK_LEFT:
                primary_ray.position += rotation*vec4(-0.5,0,0,0);
                position += rotation*vec4(-0.1f,0,0,0);
                break;
            case SDLK_UP:
                primary_ray.position += rotation*vec4(0,-0.1f,0,0);
                position += rotation*vec4(0,-0.1f,0,0);
                break;
            case SDLK_RIGHT :
                primary_ray.position += rotation*vec4(0.1f,0,0,0);
                position += rotation*(vec4(0.1f,0,0,0));
                break;
        }
    }

    void translateCamera(SDL_KeyboardEvent key){
        switch( key.keysym.sym ){
            case SDLK_UP:
                primary_ray.position += rotation*vec4(0,0,0.1f,0);
                position += rotation*vec4(0,0,0.1f,0);
                break;
            case SDLK_LEFT:
                primary_ray.position += rotation*vec4(-0.1f,0,0,0);
                position += rotation*vec4(-0.1f,0,0,0);
                break;
            case SDLK_DOWN:
                primary_ray.position += rotation*vec4(0,0,-0.1f,0);
                position += rotation*vec4(0,0,-0.1f,0);
                break;
            case SDLK_RIGHT :
                primary_ray.position += rotation*vec4(0.1f,0,0,0);
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

    Ray createNewRay(int i, int j) const {

        vec4 dir = glm::normalize(  rotation * vec4(i - SCREEN_WIDTH / 2 - position.x, 
                                    j - SCREEN_HEIGHT/ 2 - position.y, 
                                    focal_length - position.z, 1));

        return Ray(position, dir, 0);
    }

};



#endif
