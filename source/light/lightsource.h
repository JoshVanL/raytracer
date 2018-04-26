#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include <typeinfo>
#include <vector>
#include <glm/glm.hpp>
#include "../rendering/pixel.h"
#include "../rendering/rasteriser.h"

#define SCREEN_WIDTH 1500
#define SCREEN_HEIGHT 1500

#define PI           3.14159265358979323846
using glm::vec3;

class LightSource{
public:
    ////////////////////////
    glm::vec3 indirect_light = vec3(0.4f,0.4f,0.4f);
    glm::vec3 indirect_light_powperarea = vec3( 0.3f, 0.3f, 0.3f );
    //int SCREEN_WIDTH =1000;
    //int SCREEN_HEIGHT =1000;
    ////////////////////////

    glm::vec4 position;
    glm::vec3 color;

    Pixel **translatedBuffer;


    glm::vec3 power;

    LightSource(glm::vec4 pos, glm::vec3 col, vec3 pow) :
        position(pos),color(col), power(pow),translatedBuffer((Pixel**) malloc(sizeof(Pixel*) * SCREEN_HEIGHT)) {

    };
    LightSource(){
        position    = glm::vec4(0.5, 0.7, -0.1, 1.0);
        color       = glm::vec3(0.85,0.3,0.3);
        power       = vec3( 6, 3, 2 );
        translatedBuffer = ((Pixel**) malloc(sizeof(Pixel*) * SCREEN_HEIGHT));
    }

};

#endif
