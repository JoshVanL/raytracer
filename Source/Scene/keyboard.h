#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "SDL.h"
#include "../Light/lightsource.h"

class Keyboard {
public:

    bool LCTRL = false;
    Keyboard(){

    }

    void translateLight(SDL_KeyboardEvent key, LightSource& lightSource){
        switch( key.keysym.sym ){
            case SDLK_w:
                lightSource.position += vec4(0,0,0.2,0);
                break;
            case SDLK_a :
                lightSource.position += vec4(-0.2,0,0,0);
                break;
            case SDLK_s:
                lightSource.position += vec4(0,0,-0.2,0);
                break;
            case SDLK_d:
                lightSource.position += vec4(0.2,0,0,0);
                break;
        }
    }


    int ProcessKeyDown(SDL_KeyboardEvent key, LightSource lightSource, Camera& camera){
        if(key.keysym.sym == SDLK_LCTRL){
            LCTRL = true;
            return 0;
        }
        else if(key.keysym.sym == SDLK_UP || key.keysym.sym == SDLK_DOWN || key.keysym.sym == SDLK_LEFT || key.keysym.sym == SDLK_RIGHT){
            if(LCTRL)
                camera.rotateCamera(key);
            else
                camera.translateCamera(key);
            return 1;
        }
        else if(key.keysym.sym == SDLK_a || key.keysym.sym ==  SDLK_s || key.keysym.sym ==  SDLK_d || key.keysym.sym ==  SDLK_w){
            translateLight(key, lightSource);
            return 1;
        }
        else if(key.keysym.sym == SDLK_ESCAPE){
            return -1;
        }
        return 0;
    }

    void ProcessKeyUp(SDL_KeyboardEvent key){
        switch( key.keysym.sym ){
            case SDLK_LCTRL:
                if(LCTRL)
                    LCTRL = false;
                break;
            default:
                break;
        }
    }


};

#endif