#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include <typeinfo>
#include <vector>
#include <glm/glm.hpp>
#include "../rendering/pixel.h"


#define PI           3.14159265358979323846
using glm::vec3;

class LightSource{
public:
    ////////////////////////
    glm::vec3 indirect_light = vec3(0.3f,0.3f,0.3f);
    glm::vec3 indirect_light_powperarea = vec3( 0.3f, 0.3f, 0.3f );
    int SCREEN_WIDTH =1000;
    int SCREEN_HEIGHT =1000;
    ////////////////////////

    glm::vec4 position;
    glm::vec3 color;
    float depthBuffer[1400][1400];
    glm::vec3 power;

    LightSource(glm::vec4 pos, glm::vec3 col, vec3 pow) :
        position(pos),color(col), power(pow) {

    };
    LightSource(){
        position    = glm::vec4(0, 0.5, -0.7, 1.0);
        color       = glm::vec3(1,1,1);
        power       = vec3( 6, 3, 2 );
    }

    void fillDepthBuffer(){
        // Draw()
    }

    void LightDraw(const vec3& origin, vector<Shape2D*>& shapes) {
        omp_set_num_threads(16);

        memset(depthBuffer, 0, sizeof(depthBuffer));


        for ( uint32_t i = 0; i < shapes.size(); i++) {
            vector<vec4> verticies = shapes[i]->verticies();

            LightDrawPolygon(origin, verticies, shapes[i]->color, shapes[i]);
        }
    }

    void LightDrawPolygon(const vec3& origin, const vector<vec4>& vertices, const vec3 color, Shape2D* shape) {
        int V = vertices.size();
        vector<Pixel> vertexPixels( V );
        for( int i=0; i<V; i++ ) {
            Pixel::VertexShader(vertices[i], origin, vertexPixels[i], shape);
        }

        vector<Pixel> leftPixels;
        vector<Pixel> rightPixels;
        Pixel::ComputePolygonRows(origin, vertexPixels, leftPixels, rightPixels );
        LightDrawRows(origin, leftPixels, rightPixels, color );
    }

    void LightDrawLineSDL(const vec3& origin, const Pixel& a, const Pixel& b, vec3 color) {
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
                    vec3 dis = (vec3)(position) - line[i].pos3d;
                    float r = glm::length(dis);
                    r = 4.0 * 3.1415926 * r * r;

                    float result = dis.x * line[i].shape->ComputeNormal().x + 
                                dis.y * line[i].shape->ComputeNormal().y + 
                                dis.y * line[i].shape->ComputeNormal().z;

                    vec3 light_area = (result / r) * power;
                    light_area = (indirect_light_powperarea + light_area);
                    vec3 light = color * light_area;
                }
            }
        }
    }


    void LightDrawRows(const vec3& origin, const vector<Pixel>& leftPixels, const vector<Pixel>& rightPixels, const vec3 color) {
        for (int i = 0; i < leftPixels.size(); i++) {
            LightDrawLineSDL(origin, leftPixels[i], rightPixels[i], color);
        }
    }
   

};

#endif
