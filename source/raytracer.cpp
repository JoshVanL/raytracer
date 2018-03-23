#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include "scene/SDLauxiliary.h"
#include "scene/scene.h"
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
#include "light/ray.h"
#include "camera/camera.h"
#include "scene/keyboard.h"
#include "scene/kd-tree.h"
#include "scene/fractalgen.h"
#include "shapes/triangle.h"
#include "shapes/sphere.h"
#include "light/pointlight.h"
#include "light/spotlight.h"
using namespace std;

using glm::vec3;
using glm::mat3;
using glm::vec4;
using glm::mat4;


#define SCREEN_WIDTH 1800
#define SCREEN_HEIGHT 1800
#define FULLSCREEN_MODE false
#define INDIRECT_LIGHT  vec3(0.3,0.2,0.18)
#define ANG 0.1
#define NUM_THREADS 16

bool LCTRL = false;

/* ----------------------------------------------------------------------------*/
/* FUNCTIONS                                                                   */
void Update(screen* screen, SDL_Event& event, Camera& camera, vector<LightSource*> lights, Keyboard& keyboard, vector<Shape2D*>& shapes, int& runProgram, KDNode& tree);
void Draw(screen* screen, const Camera& camera, vector<LightSource*> lights, const vector<Shape2D*>& shapes, KDNode& tree);


int scount = 0;

void Draw(screen* screen, const Camera& camera, vector<LightSource*> lights, const vector<Shape2D*>& shapes, KDNode& tree) {
    memset(screen->buffer, 0, screen->height*screen->width*sizeof(uint32_t));

    omp_set_num_threads(NUM_THREADS);

    vector<vec3> line(SCREEN_HEIGHT, vec3(0, 0, 0));
    vector<vector<vec3>> colors(SCREEN_WIDTH, line);

#pragma omp parallel for schedule(static)
    for(int i=0; i<SCREEN_WIDTH; i++) {
        for(int j=0; j<SCREEN_HEIGHT; j++) {
            Ray ray     = camera.createNewRay(i,j);
            Intersection intersection;
            intersection.distance = std::numeric_limits<float>::max();
            
            if(tree.hit(ray, intersection)) {
                vec3 color(0,0,0);
                for(int k = 0; k < lights.size(); k++){
                    color += intersection.shape2D->getcolor(intersection, ray, shapes, lights[k]);
                }

                colors[i][j] = color;
            }
        }
    } 

if(camera.currentEffect != nullptr){
    vector<vec3> nline(SCREEN_HEIGHT, vec3(0, 0, 0));
    vector<vector<vec3>> ncolors(SCREEN_WIDTH, line);
    camera.currentEffect->applyCameraEffect(colors, ncolors, SCREEN_HEIGHT, SCREEN_WIDTH);
    colors.clear();
    colors = ncolors;
}    

#pragma omp parallel for schedule(static)
    for (int i = 1; i < SCREEN_WIDTH - 1; i += 2) {
        for (int j = 1; j < SCREEN_HEIGHT - 1; j += 2) {
            vec3 color = (colors[i-1][j] + colors[i+1][j]
                    + colors[i][j-1] + colors[i][j+1]) / 4;
            PutPixelSDL(screen, i/2, j/2, color);
        }
    }
}

void Update(screen* screen, SDL_Event& event, Camera& camera, vector<LightSource*> lights, Keyboard& keyboard, vector<Shape2D*>& shapes, int& runProgram, KDNode& tree){
    switch(event.type ){
        case SDL_KEYDOWN:
            keyboard.ProcessKeyDown(event.key, lights[0], camera, runProgram);
            if(runProgram == 1){
                auto started = std::chrono::high_resolution_clock::now();
                Draw(screen, camera, lights, shapes, tree);
                auto done = std::chrono::high_resolution_clock::now();
                cout << "Render time: ";
                cout << chrono::duration_cast<chrono::milliseconds>(done-started).count();
                cout << " ms \n";
                SDL_Renderframe(screen);
            }
            break;
        case SDL_KEYUP:
            keyboard.ProcessKeyUp(event.key, runProgram);
            break;
        default:
            break;
    }
}

int main( int argc, char* argv[] ) {
    // FractalGen frac(vec4(10, 10, 10, 1), vec4(80, 10, 100, 1));
    // for(int i = 0; i < 70; i++){
    //     frac.height_map.push_back(std::vector<float>());
    //     for(int j = 0; j < 90; j++){
    //         frac.height_map[i].push_back(0);
    //     }
    // }
    // frac.generate_terrain();
   
    screen *screen = InitializeSDL( SCREEN_WIDTH/2, SCREEN_HEIGHT/2, FULLSCREEN_MODE );
    vector<LightSource*> lights;
    LightSource* lightA = new PointLight();
    LightSource* lightB = new SpotLight();
    lights.push_back(lightA);
    lights.push_back(lightB);
    Camera camera(vec4(0.45, 0.5, -1.5, 1), SCREEN_WIDTH/2, CameraEffectType::NONE);
    Keyboard keyboard;
    vector<Shape2D*> shapes;
    LoadTestModel(shapes);

    SDL_Event event;
    int runProgram = 0;

    KDNode tree = *KDNode().buildTree(shapes, 0);

    auto started = std::chrono::high_resolution_clock::now();

    Draw(screen, camera, lights, shapes, tree);

    auto done = std::chrono::high_resolution_clock::now();
    cout << "Render time: ";
    cout << chrono::duration_cast<chrono::milliseconds>(done-started).count();
    cout << " ms \n";

    SDL_Renderframe(screen);

    while(runProgram != -1){
        while( SDL_PollEvent( &event ) ){
            Update(screen, event, camera, lights, keyboard, shapes, runProgram, tree);
        }
    }
    SDL_SaveImage( screen, "screenshot.bmp" );

    KillSDL(screen);
}
