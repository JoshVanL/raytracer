#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include <typeinfo>
#include <vector>
#include <glm/glm.hpp>
// #include "../rendering/rasteriser.h"
#define PI           3.14159265358979323846
using glm::vec3;

class LightSource{
public:
    ////////////////////////
    glm::vec3 indirect_light = vec3(0.3f,0.3f,0.3f);
    glm::vec3 indirect_light_powperarea = vec3( 0.3f, 0.3f, 0.3f );

    ////////////////////////

    glm::vec4 position;
    glm::vec4 direction;
    glm::vec3 color;
    // Pixel shadowBuffer[1000][1000];
    
    glm::vec3 power;

    LightSource(glm::vec4 pos, glm::vec4 direction, glm::vec3 col, vec3 pow) :
        position(pos),color(col), power(pow), direction(direction) {

    };
    LightSource(){
        position    = glm::vec4(0, 0.5, -0.7, 1.0);
        direction   = glm::vec4(0, 0, 1, 1);
        color       = glm::vec3(1,1,1);
        power       = vec3( 6, 3, 2 );
    }

    // void fillShadowBuffer(){
    //     // Draw()
    // }

    // void LightDraw(vector<Shape2D*>& shapes) {
    //     memset(shadowBuffer, 0, sizeof(shadowBuffer));
    //     for ( uint32_t i = 0; i < shapes.size(); i++) {
    //         vector<vec4> verticies = shapes[i]->verticies();
    //         LightDrawPolygon(verticies, shapes[i]->color, shapes[i]);
    //     }
    // }

    // void LightDrawPolygon(const vector<vec4>& vertices, const vec3 color, Shape2D* shape) {
    //     // int V = vertices.size();
    //     // vector<Pixel> vertexPixels( V );
    //     // for( int i=0; i<V; i++ ) {
    //     //     Rasteriser::VertexShader(vertices[i], (vec3) position, vertexPixels[i], shape);
    //     // }

    //     // vector<Pixel> leftPixels;
    //     // vector<Pixel> rightPixels;
    //     // Renderer::ProcessPolygonRows((vec3) position, vertexPixels, leftPixels, rightPixels );
    //     // LightDrawRows(leftPixels, rightPixels, color );
    // }

    // void LightDrawLineSDL(const Pixel& a, const Pixel& b, vec3 color) {
    //     int dx = abs(a.x - b.x);
    //     int dy = abs(a.y - b.y);
    //     int pixels = max(dx, dy) + 1;

    //     vector<Pixel> line(pixels);
    //     Pixel::Interpolate((vec3) position, a, b, line);
    //     for (int i = 0; i < line.size(); i++) {
    //         //Check if pixel is within viewport
    //         if(line[i].x > 0 && line[i].y > 0 && line[i].x < SCREEN_WIDTH && line[i].y < SCREEN_HEIGHT){

    //             if (line[i].zinv > shadowBuffer[line[i].x][line[i].y].zinv) {

    //                 shadowBuffer[line[i].x][line[i].y] = line[i];

    //                 vec3 dis = (vec3)(position) - line[i].pos3d;
    //                 float r = glm::length(dis);
    //                 r = 4.0 * 3.1415926 * r * r;

    //                 float result = dis.x * line[i].shape->ComputeNormal().x + 
    //                             dis.y * line[i].shape->ComputeNormal().y + 
    //                             dis.y * line[i].shape->ComputeNormal().z;

    //                 vec3 light_area = (result / r) * power;
    //                 light_area = (indirect_light_powperarea + light_area);
    //                 vec3 light = color * light_area;
    //                 shadowBuffer[line[i].x][line[i].y].light = light;
    //             }
    //         }
    //     }
    // }


    // void LightDrawRows(const vector<Pixel>& leftPixels, const vector<Pixel>& rightPixels, const vec3 color) {
    //     for (int i = 0; i < leftPixels.size(); i++) {
    //         LightDrawLineSDL(leftPixels[i], rightPixels[i], color);
    //     }
    // }
   

};

#endif
