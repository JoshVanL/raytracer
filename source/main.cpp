#include <stdio.h>
#include <omp.h>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <vector>
#include <iostream>
#include <cassert>
#include <chrono>

#include <glm/gtx/norm.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "scene/SDLauxiliary.h"
#include "scene/scene.h"
#include "scene/camera.h"
#include "scene/keyboard.h"
#include "scene/kd-tree.h"
#include "shapes/triangle.h"
#include "shapes/sphere.h"
#include "light/lightsource.h"
#include "rendering/renderer.h"
using namespace std;

using glm::vec3;
using glm::mat3;
using glm::vec4;
using glm::mat4;
using glm::vec2;


bool LCTRL = false;

/* ----------------------------------------------------------------------------*/
/* FUNCTIONS                                                                   */

void Update(screen* screen, SDL_Event& event, Camera& camera, LightSource* lightSource, Keyboard& keyboard, vector<Shape2D*>& shapes, int& runProgram){
    switch(event.type ){
        case SDL_KEYDOWN:
            keyboard.ProcessKeyDown(event.key, lightSource, runProgram);
            if(runProgram == 1){
                auto started = std::chrono::high_resolution_clock::now();
                Renderer::Draw(screen, (vec3) camera.position, lightSource, shapes, true);
                auto done = std::chrono::high_resolution_clock::now();
                cout << "Render time: ";
                cout << chrono::duration_cast<chrono::milliseconds>(done-started).count();
                cout << " ms \n";
                SDL_Renderframe(screen);
            }
            break;
        case SDL_KEYUP:
            keyboard.ProcessKeyUp(event.key);
            break;
        default:
            break;
    }
}

int main( int argc, char* argv[] ) {
    screen *screen = InitializeSDL( SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN_MODE );

    LightSource* spotLight = new LightSource();
    Camera camera(vec4(0.45, 0.5, -2.0, 1), focal_length);
    Keyboard keyboard(camera);
    vector<Shape2D*> shapes;
    LoadTestModel(shapes);

    SDL_Event event;
    int runProgram = 0;
    
    auto started = std::chrono::high_resolution_clock::now();
    Renderer::Draw(screen, (vec3) camera.position, spotLight, shapes, true);

    auto done = std::chrono::high_resolution_clock::now();
    cout << "Render time: ";
    cout << chrono::duration_cast<chrono::milliseconds>(done-started).count();
    cout << " ms \n";

    SDL_Renderframe(screen);

    while(runProgram != -1){
        while( SDL_PollEvent( &event ) ){
            Update(screen, event, camera, spotLight, keyboard, shapes, runProgram);
        }
    }
    SDL_SaveImage( screen, "screenshot.bmp" );

    KillSDL(screen);
}
