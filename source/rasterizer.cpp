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
#include "light/pointlight.h"
#include "rendering/pixel.h"

using namespace std;

using glm::vec3;
using glm::mat3;
using glm::vec4;
using glm::mat4;
using glm::vec2;

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000
#define FULLSCREEN_MODE false
#define INDIRECT_LIGHT vec3(0.3,0.3,0.3)
#define ANG 0.1
#define NUM_THREADS 16
#define LIGHTPOS vec3(0,0.5,-0.7)
#define LIGHTPOWER vec3( 6, 3, 2 )
#define INDIRECTLIGHTPOWERPERAREA vec3( 0.3f, 0.3f, 0.3f )


bool LCTRL = false;
// because we go off the screen we have a bit of border here
float depthBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];

const float focal_length = SCREEN_WIDTH;


/* ----------------------------------------------------------------------------*/
/* FUNCTIONS                                                                   */
void Update(screen* screen, SDL_Event& event, const vec3& origin, LightSource* lightSource, Keyboard& keyboard, vector<Shape2D*>& shapes, int& runProgram);
void Draw(screen* screen, const vec3& origin, LightSource* lightSource, vector<Shape2D*>& shapes);
void DrawLineSDL(screen* screen, LightSource* lightSource,const Pixel& a, const Pixel& b, vec3 color);
void DrawRows(screen *screen, LightSource* lightSource,const vec3& origin, const vector<Pixel>& leftPixels, const vector<Pixel>& rightPixels, const vec3 color);
void DrawPolygon( screen *screen, LightSource* lightSource,const vec3& origin, const vector<vec4>& verticies, const vec3 color, Shape2D* shape);


void Draw(screen* screen, const vec3& origin, LightSource* lightSource, vector<Shape2D*>& shapes) {
    memset(screen->buffer, 0, screen->height*screen->width*sizeof(uint32_t));

    omp_set_num_threads(NUM_THREADS);

    memset(depthBuffer, 0, sizeof(depthBuffer));


    for ( uint32_t i = 0; i < shapes.size(); i++) {
        vector<vec4> verticies = shapes[i]->verticies();

        DrawPolygon(screen, lightSource, origin, verticies, shapes[i]->color, shapes[i]);
    }
}

void DrawPolygon( screen *screen, LightSource* lightSource, const vec3& origin, const vector<vec4>& vertices, const vec3 color, Shape2D* shape) {
    int V = vertices.size();
    vector<Pixel> vertexPixels( V );
    for( int i=0; i<V; i++ ) {
        Pixel::VertexShader(vertices[i], origin, vertexPixels[i], shape);
    }

    vector<Pixel> leftPixels;
    vector<Pixel> rightPixels;
    Pixel::ComputePolygonRows(origin, vertexPixels, leftPixels, rightPixels );
    DrawRows( screen, lightSource, origin, leftPixels, rightPixels, color );
}

void DrawLineSDL(screen* screen,  LightSource* lightSource, const vec3& origin, const Pixel& a, const Pixel& b, vec3 color) {
    int dx = abs(a.x - b.x);
    int dy = abs(a.y - b.y);
    int pixels = max(dx, dy) + 1;

    vector<Pixel> line(pixels);
    Pixel::Interpolate(origin, a, b, line);
    for (int i = 0; i < line.size(); i++) {
        //Check if pixel is within viewport
        if(line[i].x > 0 && line[i].y > 0 && line[i].x < SCREEN_WIDTH && line[i].y < SCREEN_HEIGHT){

            if (line[i].zinv > depthBuffer[line[i].x][line[i].y]) {

                depthBuffer[line[i].x][line[i].y] = line[i].zinv;
                vec3 dis = (vec3)(lightSource->position) - line[i].pos3d;
                float r = glm::length(dis);
                r = 4.0 * 3.1415926 * r * r;

                float result = dis.x * line[i].shape->ComputeNormal().x + dis.y * line[i].shape->ComputeNormal().y + dis.y * line[i].shape->ComputeNormal().z;

                vec3 light_area = result / r * LIGHTPOWER;
                light_area = (INDIRECTLIGHTPOWERPERAREA + light_area);
                vec3 light = color * light_area;


                PutPixelSDL(screen, line[i].x, line[i].y, light);
            }
        }
    }
}


void DrawRows(screen *screen, LightSource* lightSource, const vec3& origin, const vector<Pixel>& leftPixels, const vector<Pixel>& rightPixels, const vec3 color) {
    for (int i = 0; i < leftPixels.size(); i++) {
        DrawLineSDL(screen, lightSource, origin, leftPixels[i], rightPixels[i], color);
    }
}

void Update(screen* screen, SDL_Event& event, Camera& camera, LightSource* lightSource, Keyboard& keyboard, vector<Shape2D*>& shapes, int& runProgram){
    switch(event.type ){
        case SDL_KEYDOWN:
            keyboard.ProcessKeyDown(event.key, lightSource, runProgram);
            if(runProgram == 1){
                auto started = std::chrono::high_resolution_clock::now();
                Draw(screen, (vec3) camera.position, lightSource, shapes);
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
    Draw(screen, (vec3) camera.position, spotLight, shapes);

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
