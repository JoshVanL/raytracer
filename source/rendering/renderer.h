#ifndef RENDEREH_H
#define RENDEREH_H

#include "../scene/SDLauxiliary.h"
#include "pixel.h"
#include "rasteriser.h"
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

// Pixel depthBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
// glm::vec3 frameBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
// const float focal_length = SCREEN_WIDTH;
class Renderer {

public:
    

    Renderer(){

    }

    // static void Draw(screen* screen, const vec3& origin, LightSource* lightSource, vector<Shape2D*>& shapes, bool draw = true) {
    //     memset(screen->buffer, 0, screen->height*screen->width*sizeof(uint32_t));

    //     omp_set_num_threads(NUM_THREADS);

    //     memset(&depthBuffer, 0, sizeof(depthBuffer));

    //     for ( uint32_t i = 0; i < shapes.size(); i++) {
    //         vector<vec4> verticies = shapes[i]->verticies();
    //         ProcessPolygonLines(screen, lightSource, origin, verticies, shapes[i]->color, shapes[i]);
    //     }
    //     ProcessColor();
    //     ProcessLight(lightSource);
    //     if(draw){
    //         RenderFrameBuffer(screen);
    //     };
    // }

    // static void ProcessPolygonLines( screen *screen, LightSource* lightSource, const vec3& origin, const vector<vec4>& vertices, const vec3 color, Shape2D* shape) {
    //     int V = vertices.size();
    //     vector<Pixel> vertexPixels( V );
    //     for( int i=0; i<V; i++ ) {
    //         Rasteriser::VertexShader(vertices[i], origin, vertexPixels[i], shape);
    //     }

    //     vector<Pixel> leftPixels;
    //     vector<Pixel> rightPixels;
    //     ProcessPolygonRows(origin, vertexPixels, leftPixels, rightPixels );
    //     for (int i = 0; i < leftPixels.size(); i++) {
    //         ProcessPolygonLine(screen, lightSource, origin, leftPixels[i], rightPixels[i], color);
    //     }
    // }
    // // @: Computes 2 vectors 'leftPixels' & 'rightPixels' storing the min & max x-value respectively, for each horizontal row the shape occupies
    // static void ProcessPolygonRows(const vec3& origin, const vector<Pixel>& vertexPixels, vector<Pixel>& leftPixels, vector<Pixel>& rightPixels) {
    //     int minY = vertexPixels[0].y;
    //     int maxY = vertexPixels[0].y;
        
    //     /* Find max and min y-value of the polygon and compute the number of rows it occupies,
    //     where the amount of rows in a polygon is the amount of vertical space it occupies. */
    //     for (int i = 0; i < vertexPixels.size(); i++) {
    //         if (minY > vertexPixels[i].y) {
    //             minY = vertexPixels[i].y;
    //         }
    //         if (maxY < vertexPixels[i].y) {
    //             maxY = vertexPixels[i].y;
    //         }
    //     }

    //     /* Left Pixels & Right Pixels have size equal to number of rows in the polygon
    //     - Left pixels have x value equal to min x value of the row 
    //     - Right pixels have x value equal to max x value of the row  */

    //     // Initialise the right & left pixel vectors
    //     int rows = maxY - minY + 1;
    //     leftPixels = vector<Pixel>(rows);
    //     rightPixels = vector<Pixel>(rows);
    //     for (int i = 0; i < rows; i++) {
    //         //Initialize the x-coordinates in leftPixels to some really large value
    //         leftPixels[i].x     = +numeric_limits<int>::max();
    //         //Initialize the x-coordinates in rightPixels to some really small value
    //         rightPixels[i].x    = -numeric_limits<int>::max();

    //         //Resize leftPixels and rightPixels so that they have an element for each row
    //         leftPixels[i].y     = minY + i;
    //         rightPixels[i].y    = minY + i;
    //     }

    //     /* Compute the right & left pixel vectors */

    //     // - Loop through all edges of the polygon
    //     for (int i = 0; i < vertexPixels.size(); i++) {
    //         int dx = abs(vertexPixels[i].x - vertexPixels[(i + 1) % vertexPixels.size()].x);
    //         int dy = abs(vertexPixels[i].y - vertexPixels[(i + 1) % vertexPixels.size()].y);
    //         int pixels = max(dx, dy) + 1;

    //         // Use linear interpolation to find the x-coordinate for each row it occupies.
    //         vector<Pixel>line = vector<Pixel> (pixels);
    //         Pixel::Interpolate(origin, vertexPixels[i], vertexPixels[(i + 1) % vertexPixels.size()], line);
            
    //         //Update the corresponding values in rightPixels and leftPixels.
    //         for (int r = 0; r < rows; r++) {
    //             for (int l = 0; l < line.size(); l++) {

    //                 if (line[l].y == minY + r) {
    //                     if (line[l].x < leftPixels[r].x) {
    //                         leftPixels[r].x = line[l].x;
    //                         leftPixels[r].zinv = line[l].zinv;
    //                         leftPixels[r].shape = line[l].shape;
    //                     }

    //                     if (line[l].x > rightPixels[r].x) {
    //                         rightPixels[r].x = line[l].x;
    //                         rightPixels[r].zinv = line[l].zinv;
    //                         rightPixels[r].shape = line[l].shape;
    //                     }
    //                 }
    //             }
    //         }
    //     }
    // }
    // static void ProcessPolygonLine(screen* screen,  LightSource* lightSource, const vec3& origin, const Pixel& a, const Pixel& b, vec3 color) {
    //     int dx = abs(a.x - b.x);
    //     int dy = abs(a.y - b.y);
    //     int pixels = max(dx, dy) + 1;

    //     vector<Pixel> line(pixels);
    //     Pixel::Interpolate(origin, a, b, line);
    //     for (int i = 0; i < line.size(); i++) {

    //         //Clipping
    //         if( line[i].x > 0 && 
    //             line[i].y > 0 && 
    //             line[i].x < SCREEN_WIDTH && 
    //             line[i].y < SCREEN_HEIGHT){

    //             if (line[i].zinv > depthBuffer[line[i].x][line[i].y].zinv) {
    //                 depthBuffer[line[i].x][line[i].y] = line[i];
    //             }
    //         }
    //     }
    // }

    // static void ProcessColor(){
    //     memset(&frameBuffer, 0, sizeof(frameBuffer));
    //     for(int i = 0; i < SCREEN_HEIGHT; i++){
    //         for(int j = 0; j < SCREEN_WIDTH; j++){
    //             frameBuffer[i][j] = vec3(0,0,0);
    //             for(int c = max(i-1, 0); c < min(i+2,SCREEN_HEIGHT); c++){
    //                 for(int d = max(j-1, 0); d < min(j+1,SCREEN_WIDTH); d++){
    //                     if(depthBuffer[c][d].shape != nullptr)
    //                         frameBuffer[i][j] += depthBuffer[c][d].shape->color;
    //                 }
    //             }
    //             frameBuffer[i][j] /= 8.f;
    //         }
    //     }
    // }

    // static void ProcessLight(LightSource* lightSource, bool saveColorToShape = false){
    //     for(int i = 0; i < SCREEN_HEIGHT; i++){
    //         for(int j = 0; j < SCREEN_WIDTH; j++){
    //             vec3 color = frameBuffer[i][j];
    //             if(depthBuffer[i][j].shape == nullptr) {
    //                 frameBuffer[i][j] = color;
    //             }
    //             else {
    //                 vec3 dis = (vec3)(lightSource->position) - depthBuffer[i][j].pos3d;
    //                 float r = glm::length(dis);
    //                 r = 4.0 * 3.1415926 * r * r;

    //                 float result =  dis.x *  depthBuffer[i][j].shape->ComputeNormal().x + 
    //                                 dis.y *  depthBuffer[i][j].shape->ComputeNormal().y + 
    //                                 dis.z *  depthBuffer[i][j].shape->ComputeNormal().z;

    //                 vec3 light_area = result / r * LIGHTPOWER;
    //                 light_area = (INDIRECTLIGHTPOWERPERAREA + light_area);
    //                 frameBuffer[i][j] = color * light_area; 
    //             }
    //         }
    //     }
    // }

    // static void RenderFrameBuffer(screen* screen){
    //     for(int i = 0; i < SCREEN_HEIGHT; i++){
    //         for(int j = 0; j < SCREEN_WIDTH; j++){
    //             PutPixelSDL(screen, i, j, frameBuffer[i][j]);
    //         }
    //     }
    // }

};

#endif 
