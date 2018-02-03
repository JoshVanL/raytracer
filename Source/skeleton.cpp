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

typedef struct Intersection{
    vec4 position;
    float distance;
    int triangleIndex;
    Triangle* triangle;
} Intersection;

typedef struct LightSource{
    vec4 position;
    vec3 color;
    float power;
} LightSource;

typedef struct Camera{
    vec4 position;
    float focalLength;
    std::map<int, std::map<int, Triangle*>> depthMap;
} Camera;

typedef struct	BSPTree
{
   vec3 plane[3];
   vector<Triangle> triangles;
   BSPTree  *front,
            *back;
} BSPTree;

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
    printf("\rProgress:%3d%% ", (int) (percentage * 100));
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

bool IsOccluded(Intersection point, LightSource lightSource, Triangle triangle){

    vec4 t_p = point.triangle->v0;
    vec4 u_p = point.triangle->v1;
    vec4 v_p = point.triangle->v2;
    vec4 dir = lightSource.position - point.position;

    vec3 intersect = GetIntersection(point.position, dir, triangle);

    float t = intersect.x;
    float u = intersect.y;
    float v = intersect.z;
    
    if(0 < t && 0 <= u && 0 <= v && u + v <= 1 && (t_p != triangle.v0 || u_p != triangle.v1 || v_p != triangle.v2))
        return true;

    return false;
}

vec3 GetLightIntensity(LightSource lightSource, Intersection point, vector<Triangle> triangles){

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

vec3 GetLightIntensity(LightSource lightSource, Intersection point, Triangle triangle){

    /* If occluded, return a shadow */
    if (IsOccluded(point, lightSource, triangle)) {
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

void ZInitialise(Camera camera, vector<Triangle> triangles) {
    for(int i=0; i<SCREEN_WIDTH; i++) {
        for(int j=0; j<SCREEN_HEIGHT; j++) {
            vec4 dir = vec4(i - SCREEN_WIDTH/2 - camera.position.x, 
                            j - SCREEN_HEIGHT/2 - camera.position.y, 
                            camera.focalLength - camera.position.z, 
                            1);
            Intersection intersection;
            if (ClosestIntersection(camera.position, dir, triangles, intersection)) {
                camera.depthMap[i][j] = intersection.triangle;// .insert(make_pair(i, make_pair(j,intersection.triangle) ));
            }
        }
    }
}

void Draw(screen* screen, const Camera camera, const LightSource lightSource, vector<Triangle> triangles) {

    memset(screen->buffer, 0, screen->height*screen->width*sizeof(uint32_t));

    for(int i=0; i<SCREEN_WIDTH; i++) {
        for(int j=0; j<SCREEN_HEIGHT; j++) {

            vec4 dir = vec4(i - SCREEN_WIDTH/2 - camera.position.x, 
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
    }
    fflush(stdout);
}

void Update()
{
    static int t = SDL_GetTicks();
    int t2 = SDL_GetTicks();
    float dt = float(t2-t);
    t = t2;
}

bool ProcessKeyDown(SDL_KeyboardEvent key, LightSource& lightSource, Camera& camera){
    switch( key.keysym.sym ){
        /* MOVE LIGHT SOURCE */
        case SDLK_w:
           printf("W\n");
            lightSource.position += vec4(0,0,0.2,0);
            break; 
        case SDLK_a :
            printf("A\n");
            lightSource.position += vec4(-0.2,0,0,0);
            break;
        case SDLK_s:
            printf("S\n");
            lightSource.position += vec4(0,0,-0.2,0);
            break;
        case SDLK_d :
            printf("D\n");
            lightSource.position += vec4(0.2,0,0,0);
            break;
        case SDLK_ESCAPE:
            return false;
            break;
        default:
            break;
    }
    return true;
}

int main( int argc, char* argv[] )
{  
    screen *screen = InitializeSDL( SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN_MODE );

    LightSource lightSource;
    lightSource.position      = vec4(0, -0.5, -1.4, 1.0);
    lightSource.color         = vec3(1,1,1);
    lightSource.power         = 10.f;

    Camera camera;
    camera.position           = vec4(0, 0, -2.25, 1);
    camera.focalLength        = SCREEN_WIDTH/2; 

    vector<Triangle> triangles;
    LoadTestModel(triangles);


    SDL_Event event;
    bool runProgram = true;
    
    Draw(screen, camera, lightSource, triangles);
    SDL_Renderframe(screen);

    while(runProgram){
        while( SDL_PollEvent( &event ) ){
            switch( event.type ){
                case SDL_KEYDOWN:
                    runProgram = ProcessKeyDown(event.key, lightSource, camera);
                    if(!runProgram) break;
                    Draw(screen, camera, lightSource, triangles);
                    SDL_Renderframe(screen);    
                    break;
                default:
                    break;
            }
        }
    }

    SDL_SaveImage( screen, "screenshot.bmp" );

    KillSDL(screen);
    return 0;
}
