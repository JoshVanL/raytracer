#ifndef SPHERE_H
#define SPHERE_H
#include "SDL.h"
#include "shape2D.h"
#include <vector>

#define MAX_RAY_DEPTH_ 5
#define M_PI_ 3.141592653589793 

using glm::vec3;
using glm::vec4;
using glm::normalize;
using glm::dot;

using namespace std;

class Sphere : Shape2D {
public:
    glm::vec3 center;                         
    float radius, radius2;                     

    Sphere(
        glm::vec3 &c,
        float &r,
        glm::vec3 &sc,
        float &refl,
        float &transp,
        glm::vec3 &ec, 
        glm::vec3 gloss) : radius(r), radius2(r * r), center(c), Shape2D(sc, gloss, ec, transp, refl)
    {   
        
    };
    Sphere(glm::vec3 &c, float &r, glm::vec3 &sc) : Shape2D(sc, vec3(0,0,0), vec3(0,0,0), 0, 0) {
        
    };

    virtual bool intersect(Ray &ray, vec3 dir, vec4& intersection) override
    {
        vec3 rayorig = vec3(ray.position);
        dir = normalize(vec3(dir));

        vec3 L = center - rayorig;
        float tca = dot(L, dir);
        float d2 = dot(L, L) - tca * tca;
        if (d2 > radius2) { 
            return false;
        }

        float thc = sqrt(radius2 - d2);
        float t0 = tca - thc;
        float t1 = tca + thc;

        if (t0 > t1) {
            std::swap(t0, t1); 
        }
        if (t0 < 0) {
            t0 = t1; 
            if (t0 < 0) {
                return false;
            }
        }
        float t = t0;
        vec3 temp = vec3(ray.position) + (t * dir);
        intersection = vec4(temp.x, temp.y, temp.z, 1);
        return true;
    }

    virtual glm::vec3 getnormal(vec4 start, vec4 dir) override {
        glm::vec4 intersectpoint;
        if(intersect(start, dir, intersectpoint)){
            vec3 intersectDir = (vec3) intersectpoint - (vec3) start;
            vec3 intersectNormal = intersectDir - (vec3) center; 
            return glm::normalize(intersectNormal);
        }
    }
    virtual glm::vec4 tocamcoordinates(float u, float v) override{
        return vec4(center.x, center.y, center.z, 1);
    }
private:
    virtual bool isEqual(const Shape2D& other) const override {
        Sphere othersphere = static_cast<const Sphere&>(other);
        if(center == othersphere.center && radius == othersphere.radius)
            return true;
        return false;
    }
    bool intersect(vec4 start, vec4 t_dir, vec4& intersection)
    {
        vec3 rayorig = vec3(start);
        vec3 dir = normalize(vec3(t_dir));

        //Computing inside of sqrt
        vec3 L = center - rayorig;
        float tca = dot(L, dir);
        float d2 = dot(L, L) - tca * tca;
        if (d2 > radius2) { //cannot compute sqrt of negative number
            return false;
        }

        //Computing intersection points
        float thc = sqrt(radius2 - d2);
        float t0 = tca - thc;
        float t1 = tca + thc;

        //Selecting closest intersecting point not beyond the boundary of the cornels box
        if (t0 > t1) {
            std::swap(t0, t1); //pick the closer intersection
        }

        if (t0 < 0) {
            t0 = t1; // if t0 is negative, let's use t1 instead
            if (t0 < 0) {
                //printf("points outside scene\n");
                return false; // both t0 and t1 are negative
            }
        }
        float t = t0;

        vec3 temp = vec3(start) + (t * dir);
        
        intersection = vec4(temp.x, temp.y, temp.z, 1);
        return true;
    }

    // static float mix(const float &a, const float &b, const float &mix) 
    // { 
    //     return b * mix + a * (1 - mix); 
    // };
 
// static glm::vec3 trace( const glm::vec3 &rayorig, const glm::vec3 &raydir, const std::vector<Sphere> &spheres, const int &depth) 
    // { 
    //     float tnear = INFINITY_; 
    //     const Sphere* sphere = NULL; 

    //     //FIND RAY INTERSECTION
    //     for (unsigned int i = 0; i < spheres.size(); ++i) { 
    //         float t0 = INFINITY_, t1 = INFINITY_; 
    //         if (spheres[i].intersect(rayorig, raydir, t0, t1)) { 
    //             if (t0 < 0) t0 = t1; 
    //             if (t0 < tnear) { 
    //                 tnear = t0; 
    //                 sphere = &spheres[i]; 
    //             } 
    //         } 
    //     } 
    //     if (!sphere) 
    //         return glm::vec3(0,0,0); 
    //     //SET RAY INTERSECTION POINT & NORMAL OF INTERSECTION
    //     glm::vec3 surfaceColor(0,0,0); 
    //     glm::vec3 intersectPoint = rayorig + raydir * tnear; 
    //     glm::vec3 intersectNormal = intersectPoint - sphere->center; 
    //     intersectNormal = glm::normalize(intersectNormal);
       
    //     //CHECK INSIDE
    //     bool inside = false; 
    //     if (glm::dot(intersectNormal,raydir) > 0) { 
    //         intersectNormal = -intersectNormal; 
    //         inside = true; 
    //     }
    //     float bias = 1e-4;

    //     if ((sphere->transparency > 0 || sphere->reflection > 0) && depth < MAX_RAY_DEPTH_) { 
    //         //FIND REFLECTION
    //         float facingratio = -glm::dot(intersectNormal,raydir); 
    //         float fresneleffect = mix(pow(1 - facingratio, 3), 1, 0.1); 
    //         glm::vec3 refldir =  intersectNormal * glm::dot(intersectNormal, raydir);
    //         refldir *= 2; 
    //         refldir = glm::normalize(raydir - refldir); 
    //         glm::vec3 reflection = trace(intersectPoint + intersectNormal * bias, refldir, spheres, depth + 1); 

    //         //FIND REFRACTION
    //         glm::vec3 refraction(0,0,0);
    //         if (sphere->transparency) { 
    //             float ior = 1.1;
    //             float eta = (inside) ? ior : (1 / ior);
    //             float cosi = -glm::dot(raydir,intersectNormal); 
    //             float k = 1 - eta * eta * (1 - cosi * cosi); 
    //             glm::vec3 refrdir = raydir * eta + intersectNormal * (eta *  cosi - glm::sqrt(k)); 
    //             refrdir = glm::normalize(refrdir); 
    //             refraction = trace(intersectPoint - intersectNormal * bias, refrdir, spheres, depth + 1); 
    //         } 
    //         //MIX REFLECTION & REFRACTION
    //         surfaceColor = ( reflection * fresneleffect + refraction * (1 - fresneleffect) * sphere->transparency) * sphere->surfaceColor; 
    //     } 
    //     // DIFFUSE OBJECT - NO RAYTRACING REQUIRED
    //     else { 
    //         for (int i = 0; i < spheres.size(); ++i) { 
    //             if (spheres[i].emissionColor.x > 0) { 
    //                 //EMIT LIGHT FROM
    //                 glm::vec3 transmission = glm::vec3(255,255,255); 
    //                 glm::vec3 lightDirection = spheres[i].center - intersectPoint; 
    //                 lightDirection = normalize(lightDirection); 
    //                 for (int j = 0; j < spheres.size(); ++j) { 
    //                     if (i != j) { 
    //                         float t0, t1; 
    //                         if (spheres[j].intersect(intersectPoint + intersectNormal * bias, lightDirection, t0, t1)) { 
    //                             transmission = glm::vec3(0,0,0);
    //                             break; 
    //                         } 
    //                     } 
    //                 } 
    //                 surfaceColor += sphere->surfaceColor * 
    //                                 transmission * 
    //                                 std::max(float(0), glm::dot(lightDirection,intersectNormal)) * 
    //                                 spheres[i].emissionColor; 
    //             } 
    //         } 
    //     } 
    
    //     return surfaceColor + sphere->emissionColor; 
// } 

// static void render(const std::vector<Sphere> &spheres) 
    // { 
    //     int width = 640, height = 480; 
    //     glm::vec3 *image = new glm::vec3[width * height], *pixel = image; 
    //     float invWidth = 1 / float(width), invHeight = 1 / float(height); 
    //     float fov = 30, aspectratio = width / float(height); 
    //     float angle = glm::tan(M_PI * 0.5 * fov / 180.); 
    //     // Trace rays
    //     for (int y = 0; y < height; ++y) { 
    //         for (int x = 0; x < width; ++x, ++pixel) { 
    //             float xx = (2 * ((x + 0.5) * invWidth) - 1) * angle * aspectratio; 
    //             float yy = (1 - 2 * ((y + 0.5) * invHeight)) * angle; 
    //             glm::vec3 raydir(xx, yy, -1); 
    //             raydir=glm::normalize(raydir); 
    //             *pixel = trace(glm::vec3(0,0,0), raydir, spheres, 0); 
    //         } 
    //     } 
    //     // Save result to a PPM image (keep these flags if you compile under Windows)
    //     std::ofstream ofs("./untitled.ppm", std::ios::out | std::ios::binary); 
    //     ofs << "P6\n" << width << " " << height << "\n255\n"; 
    //     for (int i = 0; i < width * height; ++i) { 
    //         ofs << (char)(std::min(float(1), image[i].x) * 255) << 
    //             (char)(std::min(float(1), image[i].y) * 255) << 
    //             (char)(std::min(float(1), image[i].z) * 255); 
    //     } 
    //     ofs.close(); 
    //     delete [] image; 
// } 
};
#endif