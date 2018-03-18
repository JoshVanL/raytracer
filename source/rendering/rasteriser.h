#ifndef RASTERISER_H
#define RASTERISER_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include "../scene/SDLauxiliary.h"
#include "../scene/camera.h"
#include "renderer.h"
#include "../light/lightsource.h"
using glm::vec3;

class Rasteriser 
{
public:
    glm::mat4 projection;
    glm::mat4 modelView;
    glm::mat4 viewPort;
    screen* screen;
    Camera camera;
    LightSource lightSource;

    float depth;

    Rasteriser(SDL_Surface *screen, Camera &camera,LightSource &lightSource)
        : screen(screen), camera(camera), lightSource(LightSource)
    {

    };
    void Projection(float ind) {
        projection = glm::mat4(1);
        projection[3][2] = ind;
    }
    void LookAt(vec3 eye, vec3 center, vec3 up)
    {
        vec3 z = normalize(eye - center);
        vec3 x = normalize(cross(up,z));
        vec3 y = normalize(cross(z,x));

        modelView = glm::mat4(1);
        for (int i=0; i<3; i++) {
            modelView[0][i] = x[i];
            modelView[1][i] = y[i];
            modelView[2][i] = z[i];
            modelView[i][3] = -center[i];
        }
    }
    void ViewPort(int x, int y, int w, int h){
        viewPort = glm::mat4(1);
        viewPort[0][3] = x+w/2.f;
        viewPort[1][3] = y+h/2.f;
        viewPort[2][3] = depth/2.f;

        viewPort[0][0] = w/2.f;
        viewPort[1][1] = h/2.f;
        viewPort[2][2] = depth/2.f;
    }

    void Rasterise(){
        ViewPort(0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1);
        vec3 centerO(0,0,0);
        vec3 up(0,1,0);
        vec3 center;

        light_pos = lightSource.position;
        camera_pos = camera.position;
	    vec3 dir = camera.direction;
        Projection(-1.0f);

        vec3 d(0,0,-1);

        LookAt(light_pos , d+light_pos , vec3(0,1,0) );
    }

};


#endif