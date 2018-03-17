#ifndef RENDEREH_H
#define RENDEREH_H

#include "../scene/SDLauxiliary.h"
#include "pixel.h"
#include <SDL2/SDL.h>
#include <omp.h>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000
#define FULLSCREEN_MODE false
#define INDIRECT_LIGHT vec3(0.3,0.3,0.3)
#define ANG 0.1
#define NUM_THREADS 16
#define LIGHTPOS vec3(0,0.5,-0.7)
#define LIGHTPOWER vec3( 6, 3, 2 )
#define INDIRECTLIGHTPOWERPERAREA vec3( 0.3f, 0.3f, 0.3f )

using namespace std;

float depthBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
const float focal_length = SCREEN_WIDTH;
class Renderer {

public:

    Renderer(){

    }
    static void Draw(screen* screen, const vec3& origin, LightSource* lightSource, vector<Shape2D*>& shapes) {
        memset(screen->buffer, 0, screen->height*screen->width*sizeof(uint32_t));

        omp_set_num_threads(NUM_THREADS);

        memset(depthBuffer, 0, sizeof(depthBuffer));


        for ( uint32_t i = 0; i < shapes.size(); i++) {
            vector<vec4> verticies = shapes[i]->verticies();

            DrawPolygon(screen, lightSource, origin, verticies, shapes[i]->color, shapes[i]);
        }
    }

    static void DrawPolygon( screen *screen, LightSource* lightSource, const vec3& origin, const vector<vec4>& vertices, const vec3 color, Shape2D* shape) {
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

    static void DrawLineSDL(screen* screen,  LightSource* lightSource, const vec3& origin, const Pixel& a, const Pixel& b, vec3 color) {
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

                    float result = dis.x * line[i].shape->ComputeNormal().x + 
                                dis.y * line[i].shape->ComputeNormal().y + 
                                dis.y * line[i].shape->ComputeNormal().z;

                    vec3 light_area = result / r * LIGHTPOWER;
                    light_area = (INDIRECTLIGHTPOWERPERAREA + light_area);
                    vec3 light = color * light_area;
                    PutPixelSDL(screen, line[i].x, line[i].y, light);
                }
            }
        }
    }


    static void DrawRows(screen *screen, LightSource* lightSource, const vec3& origin, const vector<Pixel>& leftPixels, const vector<Pixel>& rightPixels, const vec3 color) {
        for (int i = 0; i < leftPixels.size(); i++) {
            DrawLineSDL(screen, lightSource, origin, leftPixels[i], rightPixels[i], color);
        }
    }
   

};

#endif 
