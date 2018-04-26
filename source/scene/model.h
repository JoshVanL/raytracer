#ifndef MODEL_H
#define MODEL_H
// #include "../declarations.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../shapes/triangle.h"
#include "../materials/texture.h"
#include <omp.h>

using glm::ivec3;

static vec4 parse_vec3(FILE *file) {
    vec4 normal;
    fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
    normal.w = 1.0f;

    return normal;
}

static ivec3 parse_index(FILE *file) {
    vec4 normal;
    ivec3 index;
    fscanf(file, "%d/%d %d/%d %d/%d \n", &index.x, &normal.x, &index.y, &normal.y, &index.z, &normal.z);
    normal.w = 1.0f;
    //printf("%d %d %d \n", index[0], index[1], index[2]);

    return index;
}

static vec4 scale(vec4 x, vec4 min, vec4 max, vec4 a, vec4 b) {
    vec4 result;
    result.x = ((b.x-a.x)*(x.x-min.x) / (max.x-min.x)) + a.x;
    result.y = ((b.y-a.y)*(x.y-min.y) / (max.y-min.y)) + a.y;
    result.z = ((b.z-a.z)*(x.z-min.z) / (max.z-min.z)) + a.z;
    result.w = 1;
    return result;
}

static std::vector<Shape2D*> uploadModel(std::string obj_file_path, vec4 min, vec4 max){
    vector<Shape2D*> triangles;
    vector<vec4> vertices;
    vector<ivec3> indexes;
    FILE *file = fopen(obj_file_path.c_str(), "r");
    if (file == NULL) {
        std::cout << "Failed to open file " << obj_file_path << std::endl;
        exit(1);
    }


    vec4 minV = vec4(1000, 1000, 1000, 1);
    vec4 maxV = vec4(-1000, -1000, -1000, 1);

    const size_t line_size = 512;
    char line[line_size];

    while (true) {

        int read_result = fscanf(file, "%s", line);
        if (read_result == EOF) {
            break;
        }

        if (strcmp(line, "v") == 0) {
            vec4 vert = parse_vec3(file);
            vert.x = -vert.x;
            vert.z = -vert.z;
            if (vert.x < minV.x) {
               minV.x = vert.x;
            }
            if (vert.x > maxV.x) {
               maxV.x = vert.x;
            }
            if (vert.y < minV.y) {
               minV.y = vert.y;
            }
            if (vert.y > maxV.y) {
               maxV.y = vert.y;
            }
            if (vert.z < minV.z) {
               minV.z = vert.z;
            }
            if (vert.z > maxV.z) {
                maxV.z = vert.z;
            }

            vertices.push_back(vert);
        }

        if (strcmp(line, "f") == 0) {
            ivec3 index = parse_index(file);
            indexes.push_back(index);
        }
    }


    fclose(file);

    triangles.reserve(vertices.size());

    Triangle** tris = (Triangle**)malloc(sizeof(Triangle) * indexes.size()/3);

//#pragma omp parallel num_threads(4)
    for (int i=0; i<indexes.size(); i++) {
        //Triangle* tri = new Triangle(scale(vertices[i], minV, maxV, min, max), scale(vertices[i+1], minV, maxV, min, max), scale(vertices[i+2], minV, maxV, min, max), glm::vec3(0.5, 0.5, 0.5), nullptr);
        //printf("%d %d %d \n", indexes[i].x, indexes[i].y, indexes[i].z);
        Triangle* tri = new Triangle(scale(vertices[indexes[i].x], minV, maxV, min, max), scale(vertices[indexes[i].y], minV, maxV, min, max), scale(vertices[indexes[i].z], minV, maxV, min, max), glm::vec3(0.5, 0.5, 0.5), nullptr);

        tris[i] = tri;
        //printf("%d\n", i);
    }

    for (int i=0; i<indexes.size(); i++) {
        triangles.push_back(tris[i]);
    }


    return triangles;
}
#endif
