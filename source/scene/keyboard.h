#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "SDL.h"
#include "../light/lightsource.h"

class Keyboard {
public: 
    char textbuffer[200];
    bool ROTATE_SET = false;
    bool VERTICAL_SET = false;
    bool LIGHT_SET = false;
    Keyboard(){

    }

    void translateLight(SDL_KeyboardEvent key, LightSource* lightSource){
        switch( key.keysym.sym ){
            case SDLK_UP:
                lightSource->position += vec4(0,0,0.2,0);
                break;
            case SDLK_LEFT :
                lightSource->position += vec4(-0.2,0,0,0);
                break;
            case SDLK_DOWN:
                lightSource->position += vec4(0,0,-0.2,0);
                break;
            case SDLK_RIGHT:
                lightSource->position += vec4(0.2,0,0,0);
                break;
        }
    }


    void ProcessKeyDown(SDL_KeyboardEvent key, LightSource* lightSource, Camera& camera, int& runProgram){
        if(key.keysym.sym == SDLK_LCTRL){
            ROTATE_SET = true;
            runProgram = 0;
            return;
        }
        else if(key.keysym.sym == SDLK_LSHIFT){
            VERTICAL_SET = true;
            runProgram = 0;
            return;
        }
        else if(key.keysym.sym == SDLK_LALT ) {
            LIGHT_SET = true;
            runProgram = 0;
            return;
        }
        else if(key.keysym.sym == SDLK_UP   || 
                key.keysym.sym == SDLK_DOWN || 
                key.keysym.sym == SDLK_LEFT || 
                key.keysym.sym == SDLK_RIGHT){
            if(ROTATE_SET)
                camera.rotateCamera(key);
            else if(VERTICAL_SET)
                camera.translateCameraVert(key);
            else if(LIGHT_SET)
                translateLight(key, lightSource);
            else 
                camera.translateCamera(key);
            runProgram = 1;
            return;
        }
        else if(key.keysym.sym == SDLK_ESCAPE){
            runProgram = -1;
            return;
        }
    }
    void ProcessKeyUp(SDL_KeyboardEvent key, int& runProgram){
        switch( key.keysym.sym ){
            case SDLK_LCTRL:
                if(ROTATE_SET)
                    ROTATE_SET = false;
                break;
            case SDLK_LSHIFT:
                if(VERTICAL_SET)
                    VERTICAL_SET = false;
                break;
            case SDLK_LALT:
                if(LIGHT_SET)
                    LIGHT_SET = false;
                break;
            default:
                break;
        }
        runProgram = 0;
    }


};

#endif
