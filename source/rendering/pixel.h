#ifndef PIXEL_H
#define PIXEL_H
#include <glm/glm.hpp>
#include "../shapes/shape2D.h"
#include <vector>


using glm::vec3;
using glm::vec4;
using namespace std;
class Pixel {
public:
    int x, y;
    float zinv;
    vec3 pos3d;
    Shape2D* shape;
    const static int SCREEN_WIDTH = 1000;
    const static int SCREEN_HEIGHT = 1000;
    constexpr static float focal_length = 1000.0f;
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

            pixels[i].pos3d.x = (pixels[i].x - SCREEN_WIDTH / 2) / focal_length * (pixels[i].pos3d.z - origin.z) + origin.x;
            pixels[i].pos3d.y = (pixels[i].y - SCREEN_HEIGHT / 2) / focal_length * (pixels[i].pos3d.z - origin.z) + origin.y;
            pixels[i].pos3d.z = 1 / pixels[i].zinv + origin.z;
        }
    }

    // @: Takes 4D position of a vertex v, computes 2D image position, stores it inside 2D pixel vector p = (x,y) and assigns corresponding 2d shape
    static void VertexShader(const vec4& v, const vec3& origin, Pixel& p, Shape2D* shape) {
        vec4 camPos(0.45, 0.5, -2.0, 1);

        vec4 v_prime = v - vec4(origin.x, origin.y, origin.z, 1);

        p.x = focal_length * (v_prime.x / v_prime.z) + (SCREEN_WIDTH / 2);
        p.y = focal_length * (v_prime.y / v_prime.z) + (SCREEN_HEIGHT / 2);
        p.zinv = 1 / v_prime.z;
        p.pos3d = vec3(v);
        p.shape = shape;
    }
    // static Pixel transformScreens(Pixel lightPixel, const vec4& cameraOrigin, Shape2D* shape){
    //     Pixel cameraPixel;
    //     vec4 point4d(lightPixel.pos3d, 1);
    //     // VertexShader(point4d, cameraOrigin, cameraPixel, shape);
    //     return cameraPixel;
    // }
    // @: Computes 2 vectors 'leftPixels' & 'rightPixels' storing the min & max x-value respectively, for each horizontal row the shape occupies
    static void ComputePolygonRows(const vec3& origin, const vector<Pixel>& vertexPixels, vector<Pixel>& leftPixels, vector<Pixel>& rightPixels) {
        int minY = vertexPixels[0].y;
        int maxY = vertexPixels[0].y;
        
        /* Find max and min y-value of the polygon and compute the number of rows it occupies,
        where the amount of rows in a polygon is the amount of vertical space it occupies. */
        for (int i = 0; i < vertexPixels.size(); i++) {
            if (minY > vertexPixels[i].y) {
                minY = vertexPixels[i].y;
            }
            if (maxY < vertexPixels[i].y) {
                maxY = vertexPixels[i].y;
            }
        }

        /* Left Pixels & Right Pixels have size equal to number of rows in the polygon
        - Left pixels have x value equal to min x value of the row 
        - Right pixels have x value equal to max x value of the row  */

        // Initialise the right & left pixel vectors
        int rows = maxY - minY + 1;
        leftPixels = vector<Pixel>(rows);
        rightPixels = vector<Pixel>(rows);
        for (int i = 0; i < rows; i++) {
            //Initialize the x-coordinates in leftPixels to some really large value
            leftPixels[i].x     = +numeric_limits<int>::max();
            //Initialize the x-coordinates in rightPixels to some really small value
            rightPixels[i].x    = -numeric_limits<int>::max();

            //Resize leftPixels and rightPixels so that they have an element for each row
            leftPixels[i].y     = minY + i;
            rightPixels[i].y    = minY + i;
        }

        /* Compute the right & left pixel vectors */

        // - Loop through all edges of the polygon
        for (int i = 0; i < vertexPixels.size(); i++) {
            int dx = abs(vertexPixels[i].x - vertexPixels[(i + 1) % vertexPixels.size()].x);
            int dy = abs(vertexPixels[i].y - vertexPixels[(i + 1) % vertexPixels.size()].y);
            int pixels = max(dx, dy) + 1;

            // Use linear interpolation to find the x-coordinate for each row it occupies.
            vector<Pixel>line = vector<Pixel> (pixels);
            Interpolate(origin, vertexPixels[i], vertexPixels[(i + 1) % vertexPixels.size()], line);
            
            //Update the corresponding values in rightPixels and leftPixels.
            for (int r = 0; r < rows; r++) {
                for (int l = 0; l < line.size(); l++) {

                    if (line[l].y == minY + r) {
                        if (line[l].x < leftPixels[r].x) {
                            leftPixels[r].x = line[l].x;
                            leftPixels[r].zinv = line[l].zinv;
                            leftPixels[r].shape = line[l].shape;
                        }

                        if (line[l].x > rightPixels[r].x) {
                            rightPixels[r].x = line[l].x;
                            rightPixels[r].zinv = line[l].zinv;
                            rightPixels[r].shape = line[l].shape;
                        }
                    }
                }
            }
        }
    }

};

#endif