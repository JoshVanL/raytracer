#ifndef TEXTURE_H
#define TEXTURE_H

#include "material.h"
#include <SDL.h>
#include "../Scene/SDLauxiliary.h"
using glm::vec2;
using glm::vec3;
using glm::vec4;
class Texture : public Material{
public:
    SDL_Surface *image = NULL;
    SDL_Texture *texture = NULL;
    Shape2D* shape = NULL;
    vec3 zero_zero;
    vec3 max_max;
    vec2 dimensions;
    int origformat;
    Texture(const char* imagePath) : Material("Texture"), image(SDL_LoadBMP(imagePath)){
        if(image == NULL)
            printf("NULL IMAGE\n");
    };

    Uint32 get_pixel_uint(SDL_Surface *surface, int x, int y)
    { //printf("%d %d \n", x, y);
        int bpp = surface->format->BytesPerPixel;
        /* Here p is the address to the pixel we want to retrieve */
        Uint32 *p = (Uint32 *) ( (Uint32 *) (surface->pixels) + y * surface->w + x );
      
        switch(bpp) {
        case 1:
            return *p;
            break;
        case 2:
            return *(Uint16 *)p;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
            break;

        case 4:
            return *(Uint32 *)p;
            break;

        default:
            return 0;       /* shouldn't happen, but avoids warnings */
        }
    }
    vec3 get_pixel(SDL_Surface *surface, int x, int y) {
        Uint32 pixel = get_pixel_uint(surface, x, y);

        float b = (float) (pixel & 0xFF);
        float g = (float) ((pixel >> 8) & 0xFF);
        float r = (float) ((pixel >> 16) & 0xFF);
        // printf("%d %d %d %d %d \n", x, y, r, g, b);
        return vec3( r / 255.0f, g / 255.0f, b / 255.0f);
    }
    virtual glm::vec3 material_color(Intersection& intersection, const Ray& primary_ray, const std::vector<Shape2D*>& shapes, LightSource* lightSource) override {

        Shape2D* t_shape = intersection.shape2D;
        vec3 pos = (vec3) intersection.position;
        vec2 uv = t_shape->getUV(intersection);
    
        Uint8 r, g, b;
        if(image == NULL){
            printf("NULL\n");
            return vec3(0,0,0);
        }
        float u = ((uv.x + 1.f) / 2.f) * (float) image->w;
        float v = ((uv.y + 1.f) / 2.f) * (float) image->h;
       
        vec3 pixel;

       
        pixel = get_pixel(image, u, v); 
        // cout << pixel.x << " : " <<  pixel.y << " : " << pixel.z << "\n";
        
        return pixel;
    };

 

};

#endif