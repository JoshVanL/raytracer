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
    glm::vec3 indirect_light = vec3(0.1f,0.1f,0.1f);
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
        // memset(shadowBuffer, 0, sizeof(Pixel) * SCREEN_WIDTH * SCREEN_HEIGHT);

        vec3 pos = vec3(position);

        for ( uint32_t i = 0; i < shapes.size(); i++) {
            vector<vec4> verticies = shapes[i]->verticies();
            RenderPolygon(pos, verticies, shapes[i], camera);
        }

        BuildTranslatedBuffer(pos, camera);
    }

    void BuildTranslatedBuffer(const vec3& origin, const vec3& camera) {
        // memset(translatedBuffer, 0, sizeof(translatedBuffer) * SCREEN_WIDTH * SCREEN_HEIGHT);

        //vec3 AB = origin  - camera;
        //
        for(int i = 0; i < SCREEN_HEIGHT; i++){
            for(int j = 0; j < SCREEN_WIDTH; j++){
                translatedBuffer[i][j].x = 0;
                translatedBuffer[i][j].y = 0;
                //if (translatedBuffer[i][j].x != 0 || translatedBuffer[i][j].y != 0 ) {
                //printf("%d %d\n", translatedBuffer[i][j].x, translatedBuffer[i][j].y);
                //}
            }
        }


        for(int i = 0; i < SCREEN_HEIGHT; i++){
            for(int j = 0; j < SCREEN_WIDTH; j++){
                Pixel pixel;
                VertexShader(shadowBuffer[i][j].pos3d, camera, pixel);
                //if (!shadowBuffer[i][j].pos3d.x == 0 &&  !shadowBuffer[i][j].pos3d.y == 0) {
                //    printf("%d %d %f %f %f\n", i, j, shadowBuffer[i][j].pos3d.x, shadowBuffer[i][j].pos3d.y,shadowBuffer[i][j].pos3d.y);
                //}
                //
        //static void VertexShader(const vec4& v, const vec3& origin, Pixel& p, Shape2D* shape) {
                //printf("%d %d\n", pixel.x, pixel.y);
                //translatedBuffer[pixel.x][pixel.y] = shadowBuffer[i][j];
                translatedBuffer[pixel.x][pixel.y] = pixel;
                translatedBuffer[pixel.x][pixel.y].shape = shadowBuffer[i][j].shape;
                // printf("%f : ", translatedBuffer[pixel.x][pixel.y].zinv);
                // //if (shadowBuffer[i][j].x != 0) {
                //    printf("%d %d %d %d %d %d\n", i, j, shadowBuffer[i][j].x, shadowBuffer[i][j].y, pixel.x, pixel.y);
                //}
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

        //int diffx = camera.x - origin.x;
        //int diffy = camera.y - origin.y;
        //fprintf(stdout, "%f %f %f\n", diffx, camera.x, origin.x);
        //fprintf(stdout, "%f %f %f\n", diffy, camera.y, origin.y);

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

    //static void RenderColor(){
    //    memset(&frameBuffer, 0, sizeof(frameBuffer));
    //    for(int i = 0; i < SCREEN_HEIGHT; i++){
    //        for(int j = 0; j < SCREEN_WIDTH; j++){
    //            frameBuffer[i][j] = vec3(0,0,0);
    //            for(int c = max(i-1, 0); c < min(i+2,SCREEN_HEIGHT); c++){
    //                for(int d = max(j-1, 0); d < min(j+1,SCREEN_WIDTH); d++){
    //                    if(shadowBuffer[c][d].shape != nullptr)
    //                        frameBuffer[i][j] += shadowBuffer[c][d].shape->getcolor(shadwBuffer[c][d].pos3d);
    //                }
    //            }
    //            frameBuffer[i][j] /= 8.f;
    //        }
    //    }
    //}

    //static void RenderLight(LightSource* lightSource, bool saveColorToShape = false){
    //    for(int i = 0; i < SCREEN_HEIGHT; i++){
    //        for(int j = 0; j < SCREEN_WIDTH; j++){
    //            vec3 color = frameBuffer[i][j];
    //            if(shadowBuffer[i][j].shape == nullptr) {
    //                frameBuffer[i][j] = color;
    //            }
    //            else {
    //                vec3 dis = (vec3)(lightSource->position) - shadowBuffer[i][j].pos3d;
    //                float r = glm::length(dis);
    //                r = 4.0 * 3.1415926 * r * r;

    //                float result =  dis.x *  shadowBuffer[i][j].shape->ComputeNormal().x + 
    //                                dis.y *  shadowBuffer[i][j].shape->ComputeNormal().y + 
    //                                dis.z *  shadowBuffer[i][j].shape->ComputeNormal().z;

    //                vec3 light_area = result / r * LIGHTPOWER;
    //                light_area = (INDIRECTLIGHTPOWERPERAREA + light_area);
    //                frameBuffer[i][j] = color * light_area;
    //            }
    //        }
    //    }
    //}

    // void LightDraw(vector<Shape2D*>& shapes) {
    //     memset(shadowBuffer, 0, sizeof(shadowBuffer));
    //     for ( uint32_t i = 0; i < shapes.size(); i++) {
    //         vector<vec4> verticies = shapes[i]->verticies();
    //         LightDrawPolygon(verticies, shapes[i]->color, shapes[i]);
    //     }
    // }

    // void LightDrawPolygon(const vector<vec4>& vertices, const vec3 color, Shape2D* shape) {
    //     int V = vertices.size();
    //     vector<Pixel> vertexPixels( V );
    //     for( int i=0; i<V; i++ ) {
    //         Pixel::VertexShader(vertices[i], (vec3) position, vertexPixels[i], shape);
    //     }

    //     vector<Pixel> leftPixels;
    //     vector<Pixel> rightPixels;
    //     Pixel::ComputePolygonRows((vec3) position, vertexPixels, leftPixels, rightPixels );
    //     LightDrawRows(leftPixels, rightPixels, color );
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
