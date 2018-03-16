#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "SDL2/SDL.h"

#include "../light/lightsource.h"

class Keyboard {
public:

    bool LCTRL = false;
    bool LSHIFT = false;
    Camera& camera;
    Keyboard(Camera& camera) : camera(camera){

    }

    void translateLight(SDL_KeyboardEvent key, LightSource* lightSource){
        switch( key.keysym.sym ){
            case SDLK_w:
                lightSource->position += vec4(0,0,0.2,0);
                break;
            case SDLK_a :
                lightSource->position += vec4(-0.2,0,0,0);
                break;
            case SDLK_s:
                lightSource->position += vec4(0,0,-0.2,0);
                break;
            case SDLK_d:
                lightSource->position += vec4(0.2,0,0,0);
                break;
        }
    }


    void ProcessKeyDown(SDL_KeyboardEvent key, LightSource* lightSource, int& runProgram){
        if(key.keysym.sym == SDLK_LCTRL){
            LCTRL = true;
            runProgram = 0;
            return;
        }
        else if(key.keysym.sym == SDLK_LSHIFT){
            LSHIFT = true;
            runProgram = 0;
            return;
        }
        else if(key.keysym.sym == SDLK_UP || key.keysym.sym == SDLK_DOWN || key.keysym.sym == SDLK_LEFT || key.keysym.sym == SDLK_RIGHT){
            if(LCTRL)
                camera.rotateCamera(key);
            else if(LSHIFT)
                camera.translateCameraVert(key);
            else 
                camera.translateCamera(key);
            runProgram = 1;
            return;
        }
        else if(key.keysym.sym == SDLK_a || key.keysym.sym ==  SDLK_s || key.keysym.sym ==  SDLK_d || key.keysym.sym ==  SDLK_w){
            translateLight(key, lightSource);
            runProgram = 1;
            return;
        }
        else if(key.keysym.sym == SDLK_ESCAPE){
            runProgram = -1;
            return;
        }
    }

    void ProcessKeyUp(SDL_KeyboardEvent key){
        switch( key.keysym.sym ){
            case SDLK_LCTRL:
                if(LCTRL)
                    LCTRL = false;
                break;
            case SDLK_LSHIFT:
                if(LSHIFT)
                    LSHIFT = false;
                break;
            default:
                break;
        }
    }

};

#endif
