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
#include "light/ray.h"
#include "scene/camera.h"
#include "scene/keyboard.h"
#include "scene/kd-tree.h"
#include "shapes/cuboid.h"
#include "shapes/triangle.h"
#include "shapes/sphere.h"
#include "light/pointlight.h"
#include "light/spotlight.h"

using namespace std;

using glm::vec3;
using glm::mat3;
using glm::vec4;
using glm::mat4;
using glm::vec2;

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000
#define FULLSCREEN_MODE false
#define INDIRECT_LIGHT  vec3(0.3,0.2,0.18)
#define ANG 0.1
#define NUM_THREADS 16

bool LCTRL = false;
// because we go off the screen we have a bit of border here
float depthBuffer[SCREEN_HEIGHT+200][SCREEN_WIDTH+200];

struct Pixel {
    int x;
    int y;
    float zinv;
};

/* ----------------------------------------------------------------------------*/
/* FUNCTIONS                                                                   */
void Update(screen* screen, SDL_Event& event, Camera& camera, LightSource* lightSource, Keyboard& keyboard, vector<Shape2D*>& shapes, int& runProgram);
void Draw(screen* screen, const Camera& camera, LightSource* lightSource, const vector<Shape2D*>& shapes);
void Interpolate(const Pixel& a, const Pixel& b, vector<Pixel>& result);
void DrawLineSDL(screen* screen, const Pixel& a, const Pixel& b, vec3 color);
void ComputePolygonRows( const vector<Pixel>& vertexPixels, vector<Pixel>& leftPixels, vector<Pixel>& rightPixels );
void DrawRows(screen *screen, const vector<Pixel>& leftPixels, const vector<Pixel>& rightPixels, const vec3 color);
void DrawPolygon( screen *screen, const vector<vec4>& verticies, const vec3 color);

void VertexShader( const vec4& v, Pixel& p) {
    vec4 camPos(0.45, 0.5, -2.0, 1);
    const float focal_length = SCREEN_WIDTH;

    vec4 v_prime = v - camPos;

    p.x = focal_length * (v_prime.x / v_prime.z)+ (SCREEN_WIDTH / 2);
    p.y = focal_length * (v_prime.y / v_prime.z) + (SCREEN_HEIGHT / 2);
    p.zinv = 1 / v_prime.z;
}

void Draw(screen* screen, const Camera& camera, LightSource* lightSource, const vector<Shape2D*>& shapes) {
    memset(screen->buffer, 0, screen->height*screen->width*sizeof(uint32_t));

    omp_set_num_threads(NUM_THREADS);

    memset(depthBuffer, 0, sizeof(depthBuffer));

    for ( uint32_t i = 0; i < shapes.size(); i++) {
        vector<vec4> verticies = shapes[i]->verticies();

        DrawPolygon(screen, verticies, shapes[i]->color);
    }
}

void DrawPolygon( screen *screen, const vector<vec4>& vertices, const vec3 color) {
    int V = vertices.size();
    vector<Pixel> vertexPixels( V );
    for( int i=0; i<V; i++ ) {
        VertexShader( vertices[i], vertexPixels[i] );
    }

    vector<Pixel> leftPixels;
    vector<Pixel> rightPixels;
    ComputePolygonRows( vertexPixels, leftPixels, rightPixels );
    DrawRows( screen, leftPixels, rightPixels, color );
}

void DrawLineSDL(screen* screen, const Pixel& a, const Pixel& b, vec3 color) {
    int dx = abs(a.x - b.x);
    int dy = abs(a.y - b.y);
    int pixels = max(dx, dy) + 1;

    vector<Pixel> line(pixels);
    Interpolate(a, b, line);

    for (int i = 0; i < line.size(); i++) {
        if (line[i].zinv > depthBuffer[line[i].x+100][line[i].y+100]) {
            depthBuffer[line[i].x+100][line[i].y+100] = line[i].zinv;
            PutPixelSDL(screen, line[i].x, line[i].y, color);
        }
    }
}

void ComputePolygonRows(const vector<Pixel>& vertexPixels, vector<Pixel>& leftPixels, vector<Pixel>& rightPixels) {
    int minY = vertexPixels[0].y;
    int maxY = vertexPixels[0].y;

    for (int i = 0; i < vertexPixels.size(); i++) {
        if (minY > vertexPixels[i].y) {
            minY = vertexPixels[i].y;
        }
        if (maxY < vertexPixels[i].y) {
            maxY = vertexPixels[i].y;
        }
    }

    int rows = maxY - minY + 1;
    leftPixels = vector<Pixel>(rows);
    rightPixels = vector<Pixel>(rows);

    for (int i = 0; i < rows; i++) {
        leftPixels[i].x = +numeric_limits<int>::max();
        leftPixels[i].y = minY + i;

        rightPixels[i].x = -numeric_limits<int>::max();
        rightPixels[i].y = minY + i;
    }

    for (int i = 0; i < vertexPixels.size(); i++) {
        int dx = abs(vertexPixels[i].x - vertexPixels[(i + 1) % vertexPixels.size()].x);
        int dy = abs(vertexPixels[i].y - vertexPixels[(i + 1) % vertexPixels.size()].y);
        int pixels = max(dx, dy) + 1;

        vector<Pixel>line = vector<Pixel> (pixels);
        Interpolate(vertexPixels[i], vertexPixels[(i + 1) % vertexPixels.size()], line);

        for (int r = 0; r < rows; r++) {
            for (int l = 0; l < line.size(); l++) {

                if (line[l].y == minY + r) {
                    if (line[l].x < leftPixels[r].x) {
                        leftPixels[r].x = line[l].x;
                        leftPixels[r].zinv = line[l].zinv;
                    }

                    if (line[l].x > rightPixels[r].x) {
                        rightPixels[r].x = line[l].x;
                        rightPixels[r].zinv = line[l].zinv;
                    }
                }
            }
        }
    }
}

void DrawRows(screen *screen, const vector<Pixel>& leftPixels, const vector<Pixel>& rightPixels, const vec3 color) {
    for (int i = 0; i < leftPixels.size(); i++) {
        DrawLineSDL(screen, leftPixels[i], rightPixels[i], color);
    }
}

void Update(screen* screen, SDL_Event& event, Camera& camera, LightSource* lightSource, Keyboard& keyboard, vector<Shape2D*>& shapes, int& runProgram){
    switch(event.type ){
        case SDL_KEYDOWN:
            keyboard.ProcessKeyDown(event.key, lightSource, camera, runProgram);
            if(runProgram == 1){
                auto started = std::chrono::high_resolution_clock::now();
                Draw(screen, camera, lightSource, shapes);
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

void Interpolate(const Pixel& a, const Pixel& b, vector<Pixel>& result) {
    int N = result.size();

    float sx = float(b.x - a.x) / float(max(N - 1, 1));
    float sy = float(b.y - a.y) / float(max(N - 1, 1));
    float sz = float(b.zinv - a.zinv) / float(max(N - 1, 1));

    for (int i = 0; i < N; i++) {
        result[i].x = a.x + sx * i;
        result[i].y = a.y + sy * i;
        result[i].zinv = a.zinv + sz * i;
    }
}

int main( int argc, char* argv[] ) {
    screen *screen = InitializeSDL( SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN_MODE );

    LightSource* spotLight = new SpotLight();
    Camera camera(vec4(0, 0, -2.25, 1), SCREEN_WIDTH/2);
    Keyboard keyboard;
    vector<Shape2D*> shapes;
    LoadTestModel(shapes);

    SDL_Event event;
    int runProgram = 0;

    auto started = std::chrono::high_resolution_clock::now();

    Draw(screen, camera, spotLight, shapes);

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
