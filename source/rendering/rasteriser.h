#ifndef RASTERISER_H
#define RASTERISER_H
#include <glm/glm.hpp>
#include <glm/gtx/normal.hpp>
#include "../light/lightsource.h"
#include "../scene/camera.h"
#include <math.h>
using glm::vec3;
using glm::vec2;
using glm::vec4;
class Rasteriser {
public:

    Rasteriser(){};

    static glm::mat4 ViewMatrix(vec3 eye, vec3 center, vec3 up) {
        vec3 z = glm::normalize((eye-center));
        vec3 x = glm::normalize(cross(up,z));
        vec3 y = glm::normalize(cross(z,x));
        glm::mat4 modelview(1);
        for (int i=0; i<3; i++) {
            modelview[0][i] = x[i];
            modelview[1][i] = y[i];
            modelview[2][i] = z[i];
            modelview[i][3] = -center[i];
        }     
        return modelview;
    }



    static glm::mat4 ViewPortMatrix(int x, int y, int w, int h, int depth) {

        glm::mat4 viewport(1);  //identity
        viewport[0][3] = x+w/2.f;
        viewport[1][3] = y+h/2.f;
        viewport[2][3] = depth/2.f;
        viewport[0][0] = w/2.f;
        viewport[1][1] = h/2.f;
        viewport[2][2] = depth/2.f;
      
        return viewport;          
    }   

    static glm::mat4 PerspectiveProjectionMatrix(float coeff) {
        glm::mat4 projection(1);  //identity
        projection[3][2] = coeff;        
        return projection;
    }  

    static vec3 Barycentric(vec2 A, vec2 B, vec2 C, vec2 P) {
        vec3 s[2];
        for (int i=2; i--; ) {
            s[i][0] = C[i]-A[i];
            s[i][1] = B[i]-A[i];
            s[i][2] = A[i]-P[i];
        }
        vec3 u = cross(s[0], s[1]);
        if (std::abs(u[2])>1e-2) 
            return vec3(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z);
        else
            return vec3(-1,1,1);        //discard
    }

    // @: Takes 4D position of a vertex v, computes 2D image position, stores it inside 2D pixel vector p = (x,y) and assigns corresponding 2d shape
    static void VertexShader(const vec4& v, const vec3& origin, Pixel& p, Shape2D* shape) {
        vec4 camPos(0.45, 0.5, -2.0, 1);

        vec4 v_prime = v - vec4(origin.x, origin.y, origin.z, 1);

        p.x = Camera::FOCAL_LENGTH * (v_prime.x / v_prime.z) + (Camera::SCREEN_WIDTH / 2);
        p.y = Camera::FOCAL_LENGTH * (v_prime.y / v_prime.z) + (Camera::SCREEN_HEIGHT / 2);
        p.zinv = 1 / v_prime.z;
        p.pos3d = vec3(v);
        p.shape = shape; 
    }
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
            Pixel::Interpolate(origin, vertexPixels[i], vertexPixels[(i + 1) % vertexPixels.size()], line);
            
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