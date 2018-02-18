#ifndef SPHERE_H
#define SPHERE_H
#include "SDL.h"
#include <iostream>
#include "shape3D.h"
#include <stdio.h>
#include <glm/glm.hpp>
#include <cmath>
#include <fstream> 
#include <vector>
#define MAX_RAY_DEPTH_ 5
#define M_PI_ 3.141592653589793 


class Sphere {
public:
    glm::vec3 center;                           /// position of the sphere 
    float radius, radius2;                  /// sphere radius and radius^2 
    glm::vec3 surfaceColor, emissionColor;      /// surface color and emission (light) 
    float transparency, reflection;       
    Sphere(
        const glm::vec3 &c,
        const float &r,
        const glm::vec3 &sc,
        const float &refl = 0,
        const float &transp = 0,
        const glm::vec3 &ec = glm::vec3(0,0,0)) : radius(r), radius2(r * r), center(c), surfaceColor(sc), emissionColor(ec), transparency(transp), reflection(refl)
    {   
        
    }

    bool intersect(const glm::vec3 &rayorig, const glm::vec3 &raydir, float &t0, float &t1) const
    {
        glm::vec3 l = center - rayorig;
        float tca = glm::dot(raydir, l);
        if (tca < 0) return false;
        float d2 = dot(l, l) - tca * tca;
        if (d2 > radius2) return false;
        float thc = sqrt(radius2 - d2);
        t0 = tca - thc;
        t1 = tca + thc;
        return true;
    }
    static float mix(const float &a, const float &b, const float &mix) 
    { 
        return b * mix + a * (1 - mix); 
    };
 
    static glm::vec3 trace( const glm::vec3 &rayorig, const glm::vec3 &raydir, const std::vector<Sphere> &spheres, const int &depth) 
    { 
        float tnear = INFINITY_; 
        const Sphere* sphere = NULL; 

        //FIND RAY INTERSECTION
        for (unsigned int i = 0; i < spheres.size(); ++i) { 
            float t0 = INFINITY_, t1 = INFINITY_; 
            if (spheres[i].intersect(rayorig, raydir, t0, t1)) { 
                if (t0 < 0) t0 = t1; 
                if (t0 < tnear) { 
                    tnear = t0; 
                    sphere = &spheres[i]; 
                } 
            } 
        } 
        if (!sphere) 
            return glm::vec3(0,0,0); 
        //SET RAY INTERSECTION POINT & NORMAL OF INTERSECTION
        glm::vec3 surfaceColor(0,0,0); 
        glm::vec3 intersectPoint = rayorig + raydir * tnear; 
        glm::vec3 intersectNormal = intersectPoint - sphere->center; 
        intersectNormal = glm::normalize(intersectNormal);
       
        //CHECK INSIDE
        bool inside = false; 
        if (glm::dot(intersectNormal,raydir) > 0) { 
            intersectNormal = -intersectNormal; 
            inside = true; 
        }
        float bias = 1e-4;

        if ((sphere->transparency > 0 || sphere->reflection > 0) && depth < MAX_RAY_DEPTH_) { 
            //FIND REFLECTION
            float facingratio = -glm::dot(intersectNormal,raydir); 
            float fresneleffect = mix(pow(1 - facingratio, 3), 1, 0.1); 
            glm::vec3 refldir =  intersectNormal * glm::dot(intersectNormal, raydir);
            refldir *= 2; 
            refldir = glm::normalize(raydir - refldir); 
            glm::vec3 reflection = trace(intersectPoint + intersectNormal * bias, refldir, spheres, depth + 1); 

            //FIND REFRACTION
            glm::vec3 refraction(0,0,0);
            if (sphere->transparency) { 
                float ior = 1.1;
                float eta = (inside) ? ior : (1 / ior);
                float cosi = -glm::dot(raydir,intersectNormal); 
                float k = 1 - eta * eta * (1 - cosi * cosi); 
                glm::vec3 refrdir = raydir * eta + intersectNormal * (eta *  cosi - glm::sqrt(k)); 
                refrdir = glm::normalize(refrdir); 
                refraction = trace(intersectPoint - intersectNormal * bias, refrdir, spheres, depth + 1); 
            } 
            //MIX REFLECTION & REFRACTION
            surfaceColor = ( reflection * fresneleffect + refraction * (1 - fresneleffect) * sphere->transparency) * sphere->surfaceColor; 
        } 
        // DIFFUSE OBJECT - NO RAYTRACING REQUIRED
        else { 
            for (int i = 0; i < spheres.size(); ++i) { 
                if (spheres[i].emissionColor.x > 0) { 
                    //EMIT LIGHT FROM
                    glm::vec3 transmission = glm::vec3(255,255,255); 
                    glm::vec3 lightDirection = spheres[i].center - intersectPoint; 
                    lightDirection = normalize(lightDirection); 
                    for (int j = 0; j < spheres.size(); ++j) { 
                        if (i != j) { 
                            float t0, t1; 
                            if (spheres[j].intersect(intersectPoint + intersectNormal * bias, lightDirection, t0, t1)) { 
                                transmission = glm::vec3(0,0,0);
                                break; 
                            } 
                        } 
                    } 
                    surfaceColor += sphere->surfaceColor * 
                                    transmission * 
                                    std::max(float(0), glm::dot(lightDirection,intersectNormal)) * 
                                    spheres[i].emissionColor; 
                } 
            } 
        } 
    
        return surfaceColor + sphere->emissionColor; 
    } 

    static void render(const std::vector<Sphere> &spheres) 
    { 
        int width = 640, height = 480; 
        glm::vec3 *image = new glm::vec3[width * height], *pixel = image; 
        float invWidth = 1 / float(width), invHeight = 1 / float(height); 
        float fov = 30, aspectratio = width / float(height); 
        float angle = glm::tan(M_PI * 0.5 * fov / 180.); 
        // Trace rays
        for (int y = 0; y < height; ++y) { 
            for (int x = 0; x < width; ++x, ++pixel) { 
                float xx = (2 * ((x + 0.5) * invWidth) - 1) * angle * aspectratio; 
                float yy = (1 - 2 * ((y + 0.5) * invHeight)) * angle; 
                glm::vec3 raydir(xx, yy, -1); 
                raydir=glm::normalize(raydir); 
                *pixel = trace(glm::vec3(0,0,0), raydir, spheres, 0); 
            } 
        } 
        // Save result to a PPM image (keep these flags if you compile under Windows)
        std::ofstream ofs("./untitled.ppm", std::ios::out | std::ios::binary); 
        ofs << "P6\n" << width << " " << height << "\n255\n"; 
        for (int i = 0; i < width * height; ++i) { 
            ofs << (char)(std::min(float(1), image[i].x) * 255) << 
                (char)(std::min(float(1), image[i].y) * 255) << 
                (char)(std::min(float(1), image[i].z) * 255); 
        } 
        ofs.close(); 
        delete [] image; 
    } 
};
#endif