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
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <SOIL/SOIL.h>

using glm::ivec3;
using glm::ivec4;

vector<vec4> normals;

static vec4 parse_vec3(FILE *file) {
    vec4 normal;
    fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
    normal.w = 1.0f;

    return normal;
}

static vector<ivec3> parse_index(FILE *file) {
    vec4 normal;
    vector<ivec3> index;
    char mystring [40] = {0};
    if ( fgets (mystring , 40, file) != NULL ) {
        int nums = 0;
        for(int i = 0; i < 40; i++) {
            if (mystring[i] == '/') {
                nums++;
            }
        }

        ivec3 i;
        sscanf(mystring, "%d %d %d", &i.x, &i.y, &i.z);
        index.push_back(i);

        //if (nums == 3) {
        //    ivec3 i;
        //    sscanf(mystring, "%d/%d %d/%d %d/%d", &i.x, &normal.x, &i.y, &normal.y, &i.z, &normal.z);
        //    index.push_back(i);
        //    //index.push_back(vec3(0, 0, 0));
        //    normals.push_back(normal);
        //    //printf("%d %d %d \n", i[0], i[1], i[2]);
        //} else if (nums == 4) {
        //    ivec4 i;
        //    sscanf(mystring, "%d/%d %d/%d %d/%d %d/%d", &i.x, &normal.x, &i.y, &normal.y, &i.z, &normal.z, &normal.w, &i.w);
        //    index.push_back(ivec3(i.x, i.y, i.z));
        //    //index.push_back(ivec3(i.x, i.w, i.y));
        //    //index.push_back(vec3(0, 0, 0));
        //    normals.push_back(normal);
        //}

    }


    return index;
}

static vec3 parse_texture(FILE *file) {
    vec3 tex;
    fscanf(file, "%f %f %f\n", &tex.x, &tex.y, &tex.z);

    //printf("%f %f\n", tex.x, tex.y);

    return tex;
}


static vec4 scale(vec4 x, vec4 min, vec4 max, vec4 a, vec4 b) {
    vec4 result;
    result.x = ((b.x-a.x)*(x.x-min.x) / (max.x-min.x)) + a.x;
    result.y = ((b.y-a.y)*(x.y-min.y) / (max.y-min.y)) + a.y;
    result.z = ((b.z-a.z)*(x.z-min.z) / (max.z-min.z)) + a.z;
    result.w = 1;
    return result;
}

static vec3 pixel(vec3 pos, Texture* tex) {
    float x = pos.x * tex->width;
    float y = pos.y * tex->height;
    //printf("%f %f\n", x, y);
    //printf("%f %f %d %d\n", pos.x, pos.y, tex->width, tex->height);
    return tex->get_pixel(x, y) / vec3(255, 255, 255);
}

static std::vector<Shape2D*> uploadModel(std::string obj_file_path, std::string texture_path, vec4 min, vec4 max){
    vector<Shape2D*> triangles;
    vector<vec4> vertices;
    vector<ivec3> indexes;
    vector<vec3> textures;
    FILE *file = fopen(obj_file_path.c_str(), "r");
    if (file == NULL) {
        std::cout << "Failed to open file " << obj_file_path << std::endl;
        exit(1);
    }


    vec4 minV = vec4(10000, 10000, 10000, 1);
    vec4 maxV = vec4(-10000, -10000, -10000, 1);

    const size_t line_size = 512;
    char line[line_size];

    Texture* tex = new Texture(texture_path.c_str());

    while (true) {

        bool stop = false;

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
            vector<ivec3> index = parse_index(file);
            for (int i = 0; i < index.size(); i++) {
                indexes.push_back(index[i]);
            }
        }

        if (strcmp(line, "vt") == 0) {
            textures.push_back(parse_texture(file));
        }

    }


    fclose(file);

    //bool loadOBJ(
    //    const char * path,
    //    std::vector < glm::vec3 > & out_vertices,
    //    std::vector < glm::vec2 > & out_uvs,
    //    std::vector < glm::vec3 > & out_normals
    //)

    //vector<vec3> out_vet;
    //vector<vec3> out_uvs;
    //vector<vec3> out_normals;
    //if (!loadOBJ(obj_file_path.c_str(), out_vet, out_uvs, out_normals)) {
    //    printf("HERE\n");
    //}


    //triangles.reserve(vertices.size());

    Triangle** tris = (Triangle**)malloc(sizeof(Triangle) * indexes.size()/3);

    //#pragma omp parallel num_threads(4)
    int size = 0;
    for (int i=0; i<vertices.size(); i++) {
        vec4 v1 = scale(vertices[i], minV, maxV, min, max);
        vec4 v2 = v1 + vec4(1, 1, 1, 0);
        vec4 v3 = v1 + vec4(-1, -1, -1, 0);
        Triangle* tri = new Triangle(v1, v2,v3, glm::vec3(1, 0, 0), nullptr);
        triangles.push_back(tri);
    }
    for (int i=0; i<indexes.size(); i++) {
        //if (textures[i].x != 0 && textures[i].y != 0 && textures[i].x != 1 && textures[i].y != 1 ) {
                //vec3 pix = vec3(0, 1, 0);
            //if (pix.x > 0.1  && pix.y > 0.1 && pix.z > 0.1) {
                vec4 v0 = scale(vertices[indexes[i].x], minV, maxV, min, max);
                vec4 v1 = scale(vertices[indexes[i].y], minV, maxV, min, max);
                vec4 v2 = scale(vertices[indexes[i].z], minV, maxV, min, max);
                vec3 pix = tex->material_color(vec3(v0.x, v0.y, v0.z), vec3(min.x, min.y, min.z), vec3(max.z, max.y, max.z));
                Triangle* tri = new Triangle(v0, v1, v2, pix, nullptr);

                triangles.push_back(tri);
            //}
        //}
    }

    //for (int i=0; i<indexes.size(); i++) {
    //    //Triangle* tri = new Triangle(scale(vertices[i], minV, maxV, min, max), scale(vertices[i+1], minV, maxV, min, max), scale(vertices[i+2], minV, maxV, min, max), glm::vec3(0.5, 0.5, 0.5), nullptr);
    //    //printf("%f %f\n", textures[i].x, textures[i].y);
    //    if (textures[i].x > 0 && textures[i].y != 0 ) {
    //        vec3 pix = pixel(textures[i], tex);
    //        if (pix.x > 0.1 && pix.y > 0.1 && pix.z > 0.1) {
    //            vec4 v0 = scale(vertices[indexes[i].x], minV, maxV, min, max);
    //            vec4 v1 = scale(vertices[indexes[i].y], minV, maxV, min, max);
    //            vec4 v2 = scale(vertices[indexes[i].z], minV, maxV, min, max);
    //            int lim = 100;
    //            //printf("%f %f %f\n", v0.x, v0.y, v0.z);
    //            Triangle* tri = new Triangle(v0, v1, v2, pix, nullptr);

    //            triangles.push_back(tri);
    //        }
    //    }
    //}

    return triangles;
}
#endif
