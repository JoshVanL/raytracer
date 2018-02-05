#include <iostream>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include "SDLauxiliary.h"
#include "TestModelH.h"
#include <stdio.h>
#include <map>
#include <stdint.h>
#include <cmath>
#include <stdio.h>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/normal.hpp>
using namespace std;

using glm::vec3;
using glm::mat3;
using glm::vec4;
using glm::mat4;

#define PI           3.14159265358979323846
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 256
#define FULLSCREEN_MODE false

typedef struct Intersection {
    vec4 position;
    float distance;
    int triangleIndex;
    Triangle* triangle;
} Intersection;

typedef struct LightSource {
    vec4 position;
    vec3 color;
    float power;
} LightSource;

typedef struct Camera {
    vec4 position;
    mat4 rotation;
    float yaw;
    float focalLength;
} Camera;

bool LCTRL = false;

/* ----------------------------------------------------------------------------*/
/* FUNCTIONS                                                                   */
void Update();
void Draw(screen* screen);
void PrintProgress(double percentage);


vec4 WorldToCamCoordinates(float u, float v, Triangle tri) {
    vec4 e1 = tri.v1 - tri.v0;
    vec4 e2 = tri.v2 - tri.v0;
    return tri.v0 + u * e1 + v * e2;
}

vec3 GetIntersection(vec4 start, vec4 dir, Triangle triangle){
    vec4 v0 = triangle.v0;
    vec4 v1 = triangle.v1;
    vec4 v2 = triangle.v2;
    vec3 e1 = vec3(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
    vec3 e2 = vec3(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);
    vec3 b = vec3(start.x - v0.x, start.y - v0.y, start.z - v0.z);
    vec3 d = (vec3) dir;
    mat3 A( -d, e1, e2 );
    vec3 x = glm::inverse( A ) * b;

    return x;
}

bool ClosestIntersection(vec4 start, vec4 dir, vector<Triangle> &triangles, Intersection &closestIntersection) {
    closestIntersection.distance = std::numeric_limits<float>::max();
    bool found = false;

    for (size_t i=0; i<triangles.size(); i++) {
        vec3 worldIntersectionCoordinates = GetIntersection(start, dir, triangles[i]);
        float t = worldIntersectionCoordinates.x;
        float u = worldIntersectionCoordinates.y;
        float v = worldIntersectionCoordinates.z;

        if (0 <= t && 0 <= u && 0 <= v && u + v <= 1) {
            vec4 camIntersectionCoordinates = WorldToCamCoordinates(u, v, triangles[i]);
            float d = glm::length(camIntersectionCoordinates - start);

            if (d < closestIntersection.distance) {
                closestIntersection.position = camIntersectionCoordinates;
                closestIntersection.distance = d;
                closestIntersection.triangle = &triangles[i];

                found = true;
            }
        }
    }

    return found;
}

void PrintProgress(double percentage) {
    printf("\rProgress:%3d%% ", (int) ((percentage/320) * 100));
    fflush(stdout);
}

bool IsOccluded(Intersection point, LightSource lightSource, vector<Triangle> triangles){

    vec4 t_p = point.triangle->v0;
    vec4 u_p = point.triangle->v1;
    vec4 v_p = point.triangle->v2;
    vec4 dir = lightSource.position - point.position;
    for (int i=0; i<triangles.size(); i++) {
        vec3 intersect = GetIntersection(point.position, dir, triangles[i]);

        float t = intersect.x;
        float u = intersect.y;
        float v = intersect.z;

        if(0 < t && 0 <= u && 0 <= v && u + v <= 1 &&
                (t_p != triangles[i].v0 ||
                 u_p != triangles[i].v1 ||
                 v_p != triangles[i].v2))
            return true;
    }

    return false;
}

vec3 GetLightIntensity(LightSource lightSource, Intersection point, vector<Triangle> triangles) {

    /* If occluded, return a shadow */
    if (IsOccluded(point, lightSource, triangles)) {
        return vec3(0.0001, 0.0001, 0.0001);
    }

    /* Otherwise scale the point's color by the light intensity hitting is */
    float dist = distance(lightSource.position, point.position);
    float power = lightSource.power;

    vec3 surfaceNormal = normalize(triangleNormal((vec3) point.triangle->v0, (vec3) point.triangle->v1, (vec3) point.triangle->v2));
    vec3 lightToPoint = normalize((vec3) point.position - (vec3)lightSource.position);

    float dotProduct = dot(surfaceNormal, lightToPoint);
    float powPerSurface = (power * std::max(dotProduct, 0.f))/(4 * PI * pow(dist, 2));

    return point.triangle->color * lightSource.color * powPerSurface;
}

void RotateY(mat4& rotation, float degrees){
    vec4 col1(cos(degrees), 0, -sin(degrees), 0);
    vec4 col2(0, 1, 0, 0);
    vec4 col3(sin(degrees), 0, cos(degrees), 0);
    vec4 col4(0,0,0,1);
    mat4 rotationApp = mat4(col1, col2, col3, col4);
    rotation = rotationApp * rotation;
}
void RotateX(mat4& rotation, float degrees){
    vec4 col1(1, 0, 0, 0);
    vec4 col2(0, cos(degrees), sin(degrees), 0);
    vec4 col3(0, -sin(degrees), cos(degrees), 0);
    vec4 col4(0,0,0,1);
    mat4 rotationApp = mat4(col1, col2, col3, col4);
    rotation = rotationApp * rotation;
}
void Draw(screen* screen, const Camera camera, const LightSource lightSource, vector<Triangle> triangles) {

    memset(screen->buffer, 0, screen->height*screen->width*sizeof(uint32_t));

    for(int i=0; i<SCREEN_WIDTH; i++) {
        for(int j=0; j<SCREEN_HEIGHT; j++) {

            vec4 dir = camera.rotation * vec4(i - SCREEN_WIDTH/2 - camera.position.x,
                    j - SCREEN_HEIGHT/2 - camera.position.y,
                    camera.focalLength - camera.position.z,
                    1);

            Intersection intersection;
            /* If visible to camera */
            if (ClosestIntersection(camera.position, dir, triangles, intersection)) {
                vec3 directlight = GetLightIntensity(lightSource, intersection, triangles);
                vec3 indirectLight = 0.5f * vec3(1,1,1);
                vec3 color = intersection.triangle->color * (directlight + indirectLight);
                PutPixelSDL(screen, i, j, color);

            }
        }
        PrintProgress(i+1);
    }
    printf("\rProgress: done.\n");
    fflush(stdout);
}

void Update() {
    static int t = SDL_GetTicks();
    int t2 = SDL_GetTicks();
    float dt = float(t2-t);
    t = t2;
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

void translateCamera(SDL_KeyboardEvent key,  Camera& camera){
    switch( key.keysym.sym ){
        case SDLK_UP:
            camera.position += camera.rotation*vec4(0,0,1,0);
            break;
        case SDLK_LEFT:
            camera.position += camera.rotation*vec4(-0.5,0,0,0);
            break;
        case SDLK_DOWN:
            camera.position += camera.rotation*vec4(0,0,-1,0);
            break;
        case SDLK_RIGHT :
            camera.position += camera.rotation*(vec4(0.5,0,0,0));
            break;
    }
}

void rotateCamera(SDL_KeyboardEvent key,  Camera& camera){
    switch( key.keysym.sym ){
        case SDLK_UP:
            RotateX(camera.rotation, 0.5);
            break;
        case SDLK_LEFT:
            RotateY(camera.rotation, -0.5);
            break;
        case SDLK_DOWN:
            RotateX(camera.rotation, -0.5);
            break;
        case SDLK_RIGHT :
            RotateY(camera.rotation, 0.5);
            break;
     }
}

int ProcessKeyDown(SDL_KeyboardEvent key, LightSource& lightSource, Camera& camera){
    if(key.keysym.sym == SDLK_LCTRL){
        LCTRL = true;
        return 0;
    }
    else if(key.keysym.sym == SDLK_UP || key.keysym.sym == SDLK_DOWN || key.keysym.sym == SDLK_LEFT || key.keysym.sym == SDLK_RIGHT){
        if(LCTRL)
            rotateCamera(key, camera);
        else
            translateCamera(key, camera);
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

int main( int argc, char* argv[] ) {
    screen *screen = InitializeSDL( SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN_MODE );

    LightSource lightSource;
    lightSource.position      = vec4(0, -0.5, -1.4, 1.0);
    lightSource.color         = vec3(1,1,1);
    lightSource.power         = 10.f;

    Camera camera;
    camera.rotation = mat4(vec4(1,0,0,1), vec4(0,1,0,1), vec4(0,0,1,1), vec4(0,0,0,1));
    camera.position           = vec4(0, 0, -2.25, 1);
    camera.focalLength        = SCREEN_WIDTH/2;

    vector<Triangle> triangles;
    LoadTestModel(triangles);

    SDL_Event event;
    int runProgram = 0;

    Draw(screen, camera, lightSource, triangles);
    SDL_Renderframe(screen);

    while(runProgram != -1){
        while( SDL_PollEvent( &event ) ){
            switch( event.type ){
                case SDL_KEYDOWN:
                    runProgram = ProcessKeyDown(event.key, lightSource, camera);
                    if(runProgram == -1) 
                        break;
                    else if(runProgram == 1){
                        Draw(screen, camera, lightSource, triangles);
                        SDL_Renderframe(screen);
                    }
                    break;
                case SDL_KEYUP:
                    ProcessKeyUp(event.key);
                default:
                    break;
            }
        }
    }

    SDL_SaveImage( screen, "screenshot.bmp" );

    KillSDL(screen);
    return 0;
}
