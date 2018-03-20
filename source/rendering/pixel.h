#ifndef PIXEL_H
#define PIXEL_H
#include <glm/glm.hpp>
#include "../shapes/shape2D.h"
#include "../scene/camera.h"
#include <vector>


using glm::vec3;
using glm::vec4;
using namespace std;
class Pixel {
public:
    int x, y;
    float zinv;
    glm::vec3 light = vec3(2.f, 3.f, 4.f);
    glm::vec3 pos3d;
    Shape2D* shape;

    Pixel(){

    }
    
    Pixel(int x, int y, float zinv, vec3 pos3D, Shape2D* shape2D): x(x), y(y), zinv(zinv), pos3d(pos3D), shape(shape2D){

    }

    // @: Takes 2 pixels and returns vector of pixels laying between the 2 points.
    static void Interpolate(const vec3& origin, const Pixel& a, const Pixel& b, vector<Pixel>& pixels) {
        int N = pixels.size();

        float sx = float(b.x - a.x) / float(max(N - 1, 1));
        float sy = float(b.y - a.y) / float(max(N - 1, 1));
        float sz = float(b.zinv - a.zinv) / float(max(N - 1, 1));

        for (int i = 0; i < N; i++) {
            pixels[i].x = a.x + sx * i;
            pixels[i].y = a.y + sy * i;
            pixels[i].zinv = a.zinv + sz * i;
            pixels[i].shape = a.shape;

            pixels[i].pos3d.x = (pixels[i].x - Camera::SCREEN_WIDTH / 2) / Camera::FOCAL_LENGTH * (pixels[i].pos3d.z - origin.z) + origin.x;
            pixels[i].pos3d.y = (pixels[i].y - Camera::SCREEN_HEIGHT / 2) / Camera::FOCAL_LENGTH * (pixels[i].pos3d.z - origin.z) + origin.y;
            pixels[i].pos3d.z = 1 / pixels[i].zinv + origin.z;
        }
    }

    // static Pixel transformScreens(Pixel lightPixel, const vec4& cameraOrigin, Shape2D* shape){
    //     Pixel cameraPixel;
    //     vec4 point4d(lightPixel.pos3d, 1);
    //     // VertexShader(point4d, cameraOrigin, cameraPixel, shape);
    //     return cameraPixel;
    // }
    

};

#endif