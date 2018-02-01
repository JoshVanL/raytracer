#include <iostream>
#include <glm/glm.hpp>
#include <SDL.h>
#include "SDLauxiliary.h"
#include "TestModelH.h"
#include <stdint.h>
#include <cmath>
#include <stdio.h>
using namespace std;
using glm::vec3;
using glm::mat3;
using glm::vec4;
using glm::mat4;


#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 256
#define FULLSCREEN_MODE false

typedef struct Intersection{
  vec4 position;
  float distance;
  int triangleIndex;
  Triangle* triangle;
} Intersection;

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

/*Place your drawing here*/
void Draw(screen* screen) {
    /* Clear buffer */
    memset(screen->buffer, 0, screen->height*screen->width*sizeof(uint32_t));

    vector<Triangle> triangles;
    LoadTestModel(triangles);
    float W = SCREEN_WIDTH;
    float H = SCREEN_HEIGHT;
    float focalLength = SCREEN_WIDTH/2;
    vec4 cameraPos(0, 0, -2.25, 1);

    for(int i=0; i<screen->width; i++) {
        for(int j=0; j<screen->height; j++) {
            vec3 colour(0.0,0.0,0.0);
            vec4 dir = vec4(i - W/2 - cameraPos.x, j - H/2 - cameraPos.y, focalLength - cameraPos.z, 1);
            Intersection intersection;

            if (ClosestIntersection(cameraPos, dir, triangles, intersection)) {
                colour = intersection.triangle->color;
                PutPixelSDL(screen, i, j, colour);
            }
        }
       PrintProgress((i+1) / W);
    }

    printf("\nDone.\n");
    fflush(stdout);
}

/*Place updates of parameters here*/
void Update()
{
  static int t = SDL_GetTicks();
  /* Compute frame time */
  int t2 = SDL_GetTicks();
  float dt = float(t2-t);
  t = t2;
  /*Good idea to remove this*/
  std::cout << "Render time: " << dt << " ms." << std::endl;
  /* Update variables*/
}

int main( int argc, char* argv[] )
{  
  screen *screen = InitializeSDL( SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN_MODE );

  Draw(screen);
  SDL_Renderframe(screen);
  while(NoQuitMessageSDL())

  SDL_SaveImage( screen, "screenshot.bmp" );

  KillSDL(screen);
  return 0;
}
