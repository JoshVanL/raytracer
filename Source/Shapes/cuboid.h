#ifndef CUBOID_H
#define CUBOID_H 
#include "shape.h"

class Cuboid : public Shape {
public:
    float width, height;
    float xmin, xmax, ymin, ymax, zmin, zmax;
    glm::vec3 bounds[2]; 

    Cuboid(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax, glm::vec3 surfaceColor, glm::vec3 emissionColor, float transparency, float reflection):
        Shape(center(xmin, xmax, ymin, ymax, zmin, zmax), surfaceColor, emissionColor, transparency, reflection), 
        width(width), height(height), xmin(xmin), xmax(xmax), ymin(ymin), ymax(ymax), zmin(zmin), zmax(zmax){
            bounds[0] = glm::vec3(xmin, ymin, zmin);
            bounds[1] = glm::vec3(xmax, ymax, zmax);
    };

    glm::vec3 center(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax){
        glm::vec3 a, b, c, d, e, f, g, h;
        a = glm::vec3(xmin,ymin,zmin);
        b = glm::vec3(xmax,ymin,zmin);
        c = glm::vec3(xmin,ymax,zmin);
        d = glm::vec3(xmax,ymax,zmin);
        e = glm::vec3(xmin,ymin,zmax);
        f = glm::vec3(xmax,ymin,zmax);
        g = glm::vec3(xmin,ymax,zmax);
        h = glm::vec3(xmax,ymax,zmax);
        return (a + b + c + d + e + f + g + h)/8.0f;
    }

    virtual bool intersect(const glm::vec3 &rayorig, const glm::vec3 &raydir, float &t0, float &t1) const {
        float tmin, tmax, tymin, tymax, tzmin, tzmax; 
 
        glm::vec3 inv_bound = 1.0f/raydir;
        
        tmin = (bounds[inv_bound[0] < 1].x - rayorig.x) * (inv_bound.x); 
        tmax = (bounds[1- (inv_bound[0]<1)].x - rayorig.x) * (inv_bound.x); 
        tymin = (bounds[inv_bound[1] < 1].y - rayorig.y) * (inv_bound.y); 
        tymax = (bounds[1-(inv_bound[1]<1)].y - rayorig.y)  * (inv_bound.y); 
 
        if ((tmin > tymax) || (tymin > tmax)) 
            return false; 
 
        if (tymin > tmin) 
        tmin = tymin; 
        if (tymax < tmax) 
        tmax = tymax; 
 
        tzmin = (bounds[inv_bound[2] < 1].z - rayorig.z) * inv_bound.z; 
        tzmax = (bounds[1-(inv_bound[2] < 1)].z - rayorig.z) * inv_bound.z; 
 
        if ((tmin > tzmax) || (tzmin > tmax)) 
            return false; 
 
        if (tzmin > tmin) 
        tmin = tzmin; 
        if (tzmax < tmax) 
        tmax = tzmax; 
 
        t0 = tmin; 
 
        if (t0 < 0) { 
            t0 = tmax; 
            if (t0 < 0) return false; 
        } 
 
        return true; 
    }
};



#endif 