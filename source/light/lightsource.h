#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include <typeinfo>
#include <vector>
#include <glm/glm.hpp>
#include "../rendering/pixel.h"
#include "../rendering/rasteriser.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

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

    Pixel **shadowBuffer;
    Pixel **translatedBuffer;


    glm::vec3 power;

    LightSource(glm::vec4 pos, glm::vec3 col, vec3 pow) :
        position(pos),color(col), power(pow), shadowBuffer((Pixel**) malloc(sizeof(Pixel*) * SCREEN_HEIGHT)), translatedBuffer((Pixel**) malloc(sizeof(Pixel*) * SCREEN_HEIGHT)) {

    };
    LightSource(){
        position    = glm::vec4(0.9, 0.5, -0.7, 1.0);
        color       = glm::vec3(0.8,0.3,0.3);
        power       = vec3( 6, 3, 2 );
        shadowBuffer = (Pixel**)(malloc(sizeof(Pixel*) * SCREEN_HEIGHT));
        translatedBuffer = ((Pixel**) malloc(sizeof(Pixel*) * SCREEN_HEIGHT));
    }

    void FillShadowBuffer(vector<Shape2D*>& shapes, const vec3& camera){

        for(int i = 0; i < SCREEN_HEIGHT; i++){
            shadowBuffer[i] = (Pixel*) malloc(sizeof(Pixel) * SCREEN_WIDTH);
            translatedBuffer[i] = (Pixel*) malloc(sizeof(Pixel) * SCREEN_WIDTH);
            for(int j = 0; j < SCREEN_HEIGHT; j++){
                shadowBuffer[i][j] = Pixel();
                translatedBuffer[i][j] = Pixel();
            }
        }

        vec3 pos = vec3(position);

        for ( uint32_t i = 0; i < shapes.size(); i++) {
            vector<vec4> verticies = shapes[i]->verticies();
            RenderPolygon(pos, verticies, shapes[i], camera);
        }

        BuildTranslatedBuffer(pos, camera);
    }

    void BuildTranslatedBuffer(const vec3& origin, const vec3& camera) {
        // memset(translatedBuffer, 0, sizeof(translatedBuffer) * SCREEN_WIDTH * SCREEN_HEIGHT);

        for(int i = 0; i < SCREEN_HEIGHT; i++){
            for(int j = 0; j < SCREEN_WIDTH; j++){
                translatedBuffer[i][j].x = 0;
                translatedBuffer[i][j].y = 0;
            }
        }


        for(int i = 0; i < SCREEN_HEIGHT; i++){
            for(int j = 0; j < SCREEN_WIDTH; j++){
                Pixel pixel;
                VertexShader(shadowBuffer[i][j].pos3d, camera, pixel);
                translatedBuffer[pixel.x][pixel.y] = pixel;
                translatedBuffer[pixel.x][pixel.y].shape = shadowBuffer[i][j].shape;
            }
        }
    }

    void RenderPolygon( const vec3& origin, const vector<vec4>& vertices, Shape2D* shape, const vec3& camera) {
        int V = vertices.size();
        vector<Pixel> vertexPixels( V );
        for( int i=0; i<V; i++ ) {
            Rasteriser::VertexShader(vertices[i], origin, vertexPixels[i], shape);
        }

        vector<Pixel> leftPixels;
        vector<Pixel> rightPixels;
        Rasteriser::ComputePolygonRows(origin, vertexPixels, leftPixels, rightPixels );
        for (int i = 0; i < leftPixels.size(); i++) {
            RenderLine(origin, leftPixels[i], rightPixels[i], camera);
        }
    }

    void RenderLine(const vec3& origin, const Pixel& a, const Pixel& b, const vec3& camera) {
        int dx = abs(a.x - b.x);
        int dy = abs(a.y - b.y);
        int pixels = max(dx, dy) + 1;

        vector<Pixel> line(pixels);
        Pixel::Interpolate(origin, a, b, line);
        for (int i = 0; i < line.size(); i++) {
            //Check if pixel is within viewport
            if(line[i].x > 0 && line[i].y > 0 && line[i].x < SCREEN_WIDTH && line[i].y < SCREEN_HEIGHT){
                if (line[i].zinv > shadowBuffer[line[i].x][line[i].y].zinv) {
                    shadowBuffer[line[i].x][line[i].y] = line[i];
                }
            }
        }
    }

    void VertexShader(const vec3& v, const vec3& origin, Pixel& p) {
        vec3 v_prime = v - vec3(origin.x, origin.y, origin.z);

        p.x = Camera::FOCAL_LENGTH * (v_prime.x / v_prime.z) + (SCREEN_WIDTH / 2);
        p.y = Camera::FOCAL_LENGTH * (v_prime.y / v_prime.z) + (SCREEN_HEIGHT / 2);
        p.zinv = 1 / v_prime.z;
        p.pos3d = vec3(v);
    }
};

#endif
