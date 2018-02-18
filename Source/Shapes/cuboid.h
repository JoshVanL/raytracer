#ifndef CUBOID_H
#define CUBOID_H 
#include "shape3D.h"

class Cuboid : public Shape3D {
public:
    float width, height;
    float xmin, xmax, ymin, ymax, zmin, zmax;
    glm::vec3 bounds[2]; 

    Cuboid(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax, glm::vec3 surfaceColor, glm::vec3 emissionColor, float transparency, float reflection):
        Shape3D(setcenter(xmin, xmax, ymin, ymax, zmin, zmax), surfaceColor, emissionColor, transparency, reflection), 
        xmin(xmin), xmax(xmax), ymin(ymin), ymax(ymax), zmin(zmin), zmax(zmax){
            bounds[0] = glm::vec3(xmin, ymin, zmin);
            bounds[1] = glm::vec3(xmax, ymax, zmax);
    };

    glm::vec3 setcenter(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax){
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

    static glm::vec3 normalAt(const glm::vec3 &point, const Cuboid& cuboid) {
        glm::vec3 normal;
        glm::vec3 localPoint = point - cuboid.center;
        float min = std::numeric_limits<float>::max();
        float distance = std::abs((cuboid.xmax-cuboid.xmin) - std::abs(localPoint.x));
        if (distance < min) {
            min = distance;
            normal=glm::vec3(1, 0, 0);
            normal *= (localPoint.x) > 0 ? 1 : -1;
        }
        distance = std::abs((cuboid.ymax-cuboid.ymin) - std::abs(localPoint.y));
        if (distance < min) {
            min = distance;
            normal=glm::vec3(0, 1, 0);
            normal *= (localPoint.y) > 0 ? 1 : -1;
        }
        distance = std::abs((cuboid.zmax-cuboid.zmin) - std::abs(localPoint.z));
        if (distance < min) { 
            min = distance; 
            normal=glm::vec3(0, 0, 1);
            normal *= (localPoint.z) > 0 ? 1 : -1;
        } 
        return normal;
    }

    glm::vec3 trace(const glm::vec3 &rayorig, const glm::vec3 &raydir,  const std::vector<Cuboid> &cuboids, const int &depth){
        float tnear = INFINITY_; 
        const Cuboid* cuboid = NULL; 

        //FIND RAY INTERSECTION
        for (unsigned int i = 0; i < cuboids.size(); ++i) { 
            float t0 = INFINITY_, t1 = INFINITY_; 
            if (cuboids[i].intersect(rayorig, raydir, t0, t1)) { 
                if (t0 < 0) t0 = t1; 
                if (t0 < tnear) { 
                    tnear = t0; 
                    cuboid = &cuboids[i]; 
                } 
            } 
        } 
        if (!cuboid) 
            return glm::vec3(0,0,0); 
        //SET RAY INTERSECTION POINT & NORMAL OF INTERSECTION
        glm::vec3 surfaceColor(0,0,0); 
        glm::vec3 intersectPoint = rayorig + raydir * tnear; 
        glm::vec3 intersectNormal = normalAt(intersectPoint, *cuboid);
        intersectNormal = glm::normalize(intersectNormal);
       
        //CHECK INSIDE
        bool inside = false; 
        if (glm::dot(intersectNormal,raydir) > 0) { 
            intersectNormal = -intersectNormal; 
            inside = true; 
        }
        float bias = 1e-4;

        if ((cuboid->transparency > 0 || cuboid->reflection > 0) && depth < MAX_RAY_DEPTH_) { 
            //FIND REFLECTION
            float facingratio = -glm::dot(intersectNormal,raydir); 
            float fresneleffect = mix(pow(1 - facingratio, 3), 1, 0.1); 
            glm::vec3 refldir =  intersectNormal * glm::dot(intersectNormal, raydir);
            refldir *= 2; 
            refldir = glm::normalize(raydir - refldir); 
            glm::vec3 reflection = trace(intersectPoint + intersectNormal * bias, refldir, cuboids, depth + 1); 

            //FIND REFRACTION
            glm::vec3 refraction(0,0,0);
            if (cuboid->transparency) { 
                float ior = 1.1;
                float eta = (inside) ? ior : (1 / ior);
                float cosi = -glm::dot(raydir,intersectNormal); 
                float k = 1 - eta * eta * (1 - cosi * cosi); 
                glm::vec3 refrdir = raydir * eta + intersectNormal * (eta *  cosi - glm::sqrt(k)); 
                refrdir = glm::normalize(refrdir); 
                refraction = trace(intersectPoint - intersectNormal * bias, refrdir, cuboids, depth + 1); 
            } 
            //MIX REFLECTION & REFRACTION
            surfaceColor = ( reflection * fresneleffect + refraction * (1 - fresneleffect) * cuboid->transparency) * cuboid->surfaceColor; 
        } 
        // DIFFUSE OBJECT - NO RAYTRACING REQUIRED
        else { 
            for (int i = 0; i < cuboids.size(); ++i) { 
                if (cuboids[i].emissionColor.x > 0) { 
                    //EMIT LIGHT FROM
                    glm::vec3 transmission = glm::vec3(255,255,255); 
                    glm::vec3 lightDirection = cuboids[i].center - intersectPoint; 
                    lightDirection = normalize(lightDirection); 
                    for (int j = 0; j < cuboids.size(); ++j) { 
                        if (i != j) { 
                            float t0, t1; 
                            if (cuboids[j].intersect(intersectPoint + intersectNormal * bias, lightDirection, t0, t1)) { 
                                transmission = glm::vec3(0,0,0);
                                break; 
                            } 
                        } 
                    } 
                    surfaceColor += cuboid->surfaceColor * 
                                    transmission * 
                                    std::max(float(0), glm::dot(lightDirection,intersectNormal)) * 
                                    cuboids[i].emissionColor; 
                } 
            } 
        } 
    
        return surfaceColor + cuboid->emissionColor; 
    }
};



#endif 