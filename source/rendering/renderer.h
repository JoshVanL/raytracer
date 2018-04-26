#ifndef RENDEREH_H
#define RENDEREH_H

#include "../scene/SDLauxiliary.h"
#include "rasteriser.h"
#include "pixel.h"
#include <SDL2/SDL.h>
#include <omp.h>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000
#define FULLSCREEN_MODE false
#define ANG 0.1
#define NUM_THREADS 16

#define INDIRECT_LIGHT vec3(0.3,0.3,0.3)
#define LIGHTPOS vec3(0,0.5,-0.7)
#define LIGHTPOWER vec3( 6, 3, 2 )
#define INDIRECTLIGHTPOWERPERAREA vec3( 0.3f, 0.3f, 0.3f )

using namespace std;

Pixel depthBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
glm::vec3 frameBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
const float focal_length = SCREEN_WIDTH;
class Renderer {

public:
    Renderer(){

    }

    static void Draw(screen* screen, const vec3& origin, LightSource* lightSource, vector<Shape2D*>& shapes, bool draw = true) {
        memset(screen->buffer, 0, screen->height*screen->width*sizeof(uint32_t));

        //lightSource->FillShadowBuffer(shapes, origin);

        //omp_set_num_threads(NUM_THREADS);

        memset(&depthBuffer, 0, sizeof(depthBuffer));

        for ( uint32_t i = 0; i < shapes.size(); i++) {
            vector<vec4> verticies = shapes[i]->verticies();
            RenderPolygon(screen, lightSource, origin, verticies, shapes[i]);
        }
        RenderColor();
        RenderLight(lightSource);
        if(draw){
            RenderFrameBuffer(screen);
        };
    }


    static void RenderPolygon( screen *screen, LightSource* lightSource, const vec3& origin, const vector<vec4>& vertices, Shape2D* shape) {
        int V = vertices.size();
        vector<Pixel> vertexPixels( V );
        for( int i=0; i<V; i++ ) {
            Rasteriser::VertexShader(vertices[i], origin, vertexPixels[i], shape);
        }

        vector<Pixel> leftPixels;
        vector<Pixel> rightPixels;
        Rasteriser::ComputePolygonRows(origin, vertexPixels, leftPixels, rightPixels );
        for (int i = 0; i < leftPixels.size(); i++) {
            RenderLine(screen, lightSource, origin, leftPixels[i], rightPixels[i]);
        }
    }

    static void RenderLine(screen* screen,  LightSource* lightSource, const vec3& origin, const Pixel& a, const Pixel& b) {
        int dx = abs(a.x - b.x);
        int dy = abs(a.y - b.y);
        int pixels = max(dx, dy) + 1;

        vector<Pixel> line(pixels);
        Pixel::Interpolate(origin, a, b, line);
        for (int i = 0; i < line.size(); i++) {
            //Check if pixel is within viewport
            if(line[i].x > 0 && line[i].y > 0 && line[i].x < SCREEN_WIDTH && line[i].y < SCREEN_HEIGHT){
                if (line[i].zinv > depthBuffer[line[i].x][line[i].y].zinv) {
                    depthBuffer[line[i].x][line[i].y] = line[i];
                }
            }
        }
    }

    static void RenderColor(){
        memset(&frameBuffer, 0, sizeof(frameBuffer));
        for(int i = 0; i < SCREEN_HEIGHT; i++){
            for(int j = 0; j < SCREEN_WIDTH; j++){
                frameBuffer[i][j] = vec3(0,0,0);
                for(int c = max(i-1, 0); c < min(i+2,SCREEN_HEIGHT); c++){
                    for(int d = max(j-1, 0); d < min(j+1,SCREEN_WIDTH); d++){
                        if(depthBuffer[c][d].shape != nullptr)
                            frameBuffer[i][j] += depthBuffer[c][d].shape->getcolor(depthBuffer[c][d].pos3d);
                    }
                }
                frameBuffer[i][j] /= 8.f;
            }
        }
    }

    static void RenderLight(LightSource* lightSource, bool saveColorToShape = false){
        for(int i = 0; i < SCREEN_HEIGHT; i++){
            for(int j = 0; j < SCREEN_WIDTH; j++){
                vec3 color = frameBuffer[i][j];
                if(depthBuffer[i][j].shape == nullptr) {
                    frameBuffer[i][j] = color;
                }
                else {
                    vec3 dis = (vec3)(lightSource->position) - depthBuffer[i][j].pos3d;
                    float r = glm::length(dis);
                    r = 4.0 * 3.1415926 * r * r;

                    float result =  dis.x *  depthBuffer[i][j].shape->ComputeNormal().x + 
                                    dis.y *  depthBuffer[i][j].shape->ComputeNormal().y + 
                                    dis.z *  depthBuffer[i][j].shape->ComputeNormal().z;

                    vec3 light_area = result / r * LIGHTPOWER;
                    light_area = (INDIRECTLIGHTPOWERPERAREA + light_area);
                    //if (lightSource->translatedBuffer[i][j].shape == depthBuffer[i][j].shape) {
                    //printf("%d %d\n", lightSource->translatedBuffer[i][j].x, lightSource->translatedBuffer[i][j].y);
                    //if (lightSource->translatedBuffer[i][j].zinv < depthBuffer[i][j].zinv) {
                        frameBuffer[i][j] = color * light_area;
                    //} else {
                    //    frameBuffer[i][j] = color * vec3(0.2, 0.2, 0.2);
                    //}
                }
            }
        }
    }

    static void RenderFrameBuffer(screen* screen){
        for(int i = 0; i < SCREEN_HEIGHT; i++){
            for(int j = 0; j < SCREEN_WIDTH; j++){
                PutPixelSDL(screen, i, j, frameBuffer[i][j]);
            }
        }
    }

};

#endif
