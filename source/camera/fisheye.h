#ifndef FISHEYE_H
#define FISHEYE_H
#include <glm/glm.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/gtx/norm.hpp>
#include <cmath>
#include <vector>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "camera_effect.h"
using glm::vec2;
using glm::vec3;
using glm::vec4;

class Fisheye : public CameraEffect{
public:
    double psi_max;

    Fisheye(double psi, double width, double height):psi_max(psi), CameraEffect(width, height) {

    }
    bool applyCameraEffect(std::vector<std::vector<vec3>>& srcpixels, std::vector<std::vector<vec3>>& destpixels, double SCREEN_HEIGHT, double SCREEN_WIDTH){

        for(double i=0.0; i < (double)SCREEN_WIDTH; i++) {
            for(double j=0.0; j < (double)SCREEN_HEIGHT; j++) {
                double ny = ((2.0*j)/(double)SCREEN_HEIGHT)-1.0;
                double nx = ((2.0*i)/(double)SCREEN_WIDTH)-1.0; 
                double r = sqrt(double(nx*nx) + double(ny*ny));
                if (0.0<=r&&r<=1.0) {
                    double theta = atan2(ny,nx);
                    double r_new = (r + 1.0 - sqrt(1.0 - double(r*r)))/double(2.0);
                    if(r_new <= 1.0){

                        double nxn = r_new*cos(theta);
                        double nyn = r_new*sin(theta);
                        int x2 = (int)(((nxn+1.0)*(double)SCREEN_WIDTH)/2.0); 
                        int y2 = (int)(((nyn+1.0)*(double)SCREEN_HEIGHT)/2.0); 
                        int srcpos = (int)(x2*SCREEN_HEIGHT+y2);        
                        if (srcpos>=0 && srcpos < SCREEN_WIDTH*SCREEN_HEIGHT) {
                            destpixels[x2][y2] = srcpixels[i][j];
                        }
                    }
                }
            }
        }
    }


};


#endif 