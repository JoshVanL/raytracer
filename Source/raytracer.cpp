
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include "Scene/SDLauxiliary.h"
#include "Scene/scene.h"
#include <stdio.h>
#include <omp.h>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/normal.hpp>
#include <cstdlib> 
#include <cstdio> 
#include <cmath> 
#include <fstream> 
#include <vector> 
#include <iostream> 
#include <cassert>
#include "Light/ray.h"
#include "Scene/camera.h"
#include "Scene/keyboard.h"
#include "Shapes/cuboid.h"
#include "Shapes/triangle.h"
#include "Shapes/sphere.h"

using namespace std; 

using glm::vec3;
using glm::mat3;
using glm::vec4;
using glm::mat4;


#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 256
#define FULLSCREEN_MODE false
#define INDIRECT_LIGHT  vec3(0.3,0.2,0.18)
#define ANG 0.1



bool LCTRL = false;

/* ----------------------------------------------------------------------------*/
/* FUNCTIONS                                                                   */
void Update();
void Draw(screen* screen);
void PrintProgress(double percentage);


void PrintProgress(double percentage) {
    printf("\rProgress:%3d%% ", (int) ((percentage/320) * 100));
    fflush(stdout);
}

int scount = 0;

void Draw(screen* screen, const Camera& camera, LightSource& lightSource, const vector<Shape2D*>& shapes) {
    memset(screen->buffer, 0, screen->height*screen->width*sizeof(uint32_t));

    #pragma omp parallel for
    for(int i=0; i<SCREEN_WIDTH; i++) {
        for(int j=0; j<SCREEN_HEIGHT; j++) {
            
            Ray ray     = camera.createNewRay(i,j);
            Intersection intersection;
            if(ray.ClosestIntersection(shapes, intersection)){
               
                vec3 flat_color = intersection.compute_color(ray, shapes);
                vec3 light = lightSource.light_at_position(intersection, shapes);
                vec3 final_color = flat_color * (light * intersection.shape2D->gloss);
                PutPixelSDL(screen, i, j, final_color);
            }
        }
        PrintProgress(i+1);
    }

    printf("\rProgress: done.\n");
    fflush(stdout);
}

void Update(screen* screen, SDL_Event& event, Camera& camera, LightSource& lightSource, Keyboard& keyboard, vector<Shape2D*>& shapes, int& runProgram){
    switch(event.type ){
        case SDL_KEYDOWN:
            keyboard.ProcessKeyDown(event.key, lightSource, camera, runProgram);
            if(runProgram == 1){
                Draw(screen, camera, lightSource, shapes);
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

    LightSource lightSource;
    Camera camera(vec4(0, 0, -2.25, 1), SCREEN_WIDTH/2);
    Keyboard keyboard;
    vector<Shape2D*> shapes;
    LoadTestModel(shapes);

    SDL_Event event;
    int runProgram = 0;

    Draw(screen, camera, lightSource, shapes);
    SDL_Renderframe(screen);

    while(runProgram != -1){
        while( SDL_PollEvent( &event ) ){
            Update(screen, event, camera, lightSource, keyboard, shapes, runProgram);
        }
    }
    SDL_SaveImage( screen, "screenshot.bmp" );

    KillSDL(screen);
}
