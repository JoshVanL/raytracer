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
#include <chrono>

#include "Light/ray.h"
#include "Scene/camera.h"
#include "Scene/keyboard.h"
#include "Shapes/cuboid.h"
#include "Shapes/triangle.h"
#include "Shapes/sphere.h"
#include "Shapes/polygonmesh.h"
#include "Scene/kd-tree.h"

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
void Draw(screen* screen, Camera& camera, Ray& lightSource, KDNode* tree);
void PrintProgress(double percentage);


void PrintProgress(double percentage) {
    printf("\rProgress:%3d%% ", (int) ((percentage/320) * 100));
    fflush(stdout);
}


void Draw(screen* screen, Camera& camera, Ray& lightSource, KDNode* tree) {
    memset(screen->buffer, 0, screen->height*screen->width*sizeof(uint32_t));

    #pragma omp parallel for
    for(int i=0; i<SCREEN_WIDTH; i++) {
        for(int j=0; j<SCREEN_HEIGHT; j++) {
            vec4 dir = camera.rotation * vec4(i - SCREEN_WIDTH/2 - camera.position.x,
                    j - SCREEN_HEIGHT/2 - camera.position.y,
                    camera.focal_length - camera.position.z,
                    1);
            dir = normalize(dir);

            Intersection intersection;
            if(tree->hit(tree, camera, dir, intersection)) {
                vec3 color          = (vec3) intersection.shape2D->color;
                vec3 directlight    = lightSource.GetDirectLight(intersection, tree->shapes);
                vec3 indirectlight  = lightSource.GetIndirectLight();
                color *= ((directlight + INDIRECT_LIGHT) *  (vec3) intersection.shape2D->gloss);
                PutPixelSDL(screen, i, j, color);
            }
        }
    }
}

void Update(screen* screen, SDL_Event& event, Camera& camera, Ray& lightSource, Keyboard& keyboard, KDNode* tree, int& runProgram){
    switch(event.type ){
        case SDL_KEYDOWN:
            keyboard.ProcessKeyDown(event.key, lightSource, camera, runProgram);
            if(runProgram == 1){
                Draw(screen, camera, lightSource, tree);
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

    Ray lightSource;
    Camera camera(vec4(0, 0, -2.25, 1), SCREEN_WIDTH/2);
    Keyboard keyboard;
    vector<Shape2D*> shapes;
    LoadTestModel(shapes);

    SDL_Event event;
    int runProgram = 0;

    KDNode* tree = (new KDNode())->buildTree(shapes, 0);

    auto started = std::chrono::high_resolution_clock::now();

    Draw(screen, camera, lightSource, tree);

    auto done = std::chrono::high_resolution_clock::now();
    cout << "Render time: ";
    cout << chrono::duration_cast<chrono::milliseconds>(done-started).count();
    cout << " ms \n";

    SDL_Renderframe(screen);

    while(runProgram != -1){
        while( SDL_PollEvent( &event ) ){
            Update(screen, event, camera, lightSource, keyboard, tree, runProgram);
        }
    }
    SDL_SaveImage( screen, "screenshot.bmp" );

    KillSDL(screen);
}
