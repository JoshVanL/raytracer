#ifndef FRACTALGEN_H
#define FRACTALGEN_H
#include <glm/glm.hpp>
#include <stdio.h>
#include <stdlib.h>
#include "../shapes/triangle.h"
#include <vector>
using glm::vec4;
using glm::vec3;

class FractalGen {
public:
    
    vec4 bottom_left;
    vec4 top_right;
    int width;
    int height;
    std::vector<std::vector<float> > height_map;
    std::vector<Triangle*> triangle_map;
    FractalGen(vec4 bottom_left, vec4 top_right): bottom_left(bottom_left), top_right(top_right),width(abs(top_right.x - bottom_left.x)), height(abs(top_right.y - bottom_left.y)) {
      
    }

    void generate_terrain(){
        for(int i = 0; i < width; i ++){
            for(int j = 0; j < height; j ++){
                height_map[i][j] = i * j/height;
            }
        }
             printf("## \n");
        int step = 10;
        float magnitude = 2.f;
        while(step > 4){
            diamond(step, magnitude, height_map );
                 printf("## \n");
            square(step, magnitude, height_map );
                 printf("## \n");
            step--;
            magnitude *= 0.75f;
        }
             printf("## \n");
        vec3 v0, v1, v2;
        int start_i = 0, start_j = 0;
        int i, j;
        while(true){
            v0 = vec3(start_i, start_j, height_map[start_i][start_j]);
            while(true){
                for(i = start_i+1; i < width; i ++){
                    if(height_map[i][start_j] != height_map[start_i][start_j]){
                        v1 =  vec3(bottom_left.x + i, height_map[i][start_j], bottom_left.z + start_j);
                        break;
                    }
                }
                if(i == width - 1){
                    v1 =  vec3(bottom_left.x, height_map[i-1][start_j], bottom_left.z);
                    start_i = 0;
                }
                else {
                    v1 =  vec3(bottom_left.x, height_map[i-1][start_j], bottom_left.z);
                    start_i = v1.x + 1;
                }
                break;
            }
                 printf("## \n");
            while(true){
                for(j = start_j+1; j < height; j ++){
                    if(height_map[start_i][j] != height_map[start_i][start_j]){
                            v2 =  vec3(bottom_left.x + start_i, height_map[start_i][j], bottom_left.z + j);
                            break;
                    }
                }
                if(j == height - 1){
                    v2 =  vec3(bottom_left.x, height_map[start_i][j-1], bottom_left.z);
                }
                break;
            }
                 printf("## \n");
            Triangle* tri = new Triangle(vec4(v0,1), vec4(v1,1), vec4(v2,1), vec3(  0.15f, 0.75f, 0.15f ), "terrain");
            triangle_map.push_back(tri);
            
            if(start_i == 0){
                start_j += 1;
            }
            if(start_i >= width - 1 && start_j >= height - 1){
                break;
            }
                 printf("%d %d \n", start_i, start_j);
        }
    }

    void diamond(int step, float magnitude, std::vector<std::vector<float> >& heightmap){
        int stepdouble = 2 * step;
        for(int i = stepdouble; i < width; i *=  stepdouble){
            for(int j = stepdouble; j < width; j *= stepdouble){
                printf("%d %d\n", i, j);
                heightmap[i][j] = (heightmap[i-1][j-1] + heightmap[i-1][j+1] + heightmap[i+1][j-1] + heightmap[i+1][j+1])/4;
                heightmap[i][j] +=  ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * magnitude) ;
            }
        }

    }
    void square(int step, float magnitude, std::vector<std::vector<float> >& heightmap){
        int stepdouble = 2 * step;
        for(int i = stepdouble; i < width; i *=  stepdouble){
            for(int j = stepdouble; j < width; j *= stepdouble){
                heightmap[i][j] = (heightmap[i-1][j] + heightmap[i+1][j] + heightmap[i][j-1] + heightmap[i][j+1])/4;
                heightmap[i][j] +=  (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * magnitude ;
            }
        }

    }

};

#endif 
