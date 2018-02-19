#ifndef POLYGONMESH_H
#define POLYGONMESH_H 
#include "shape3D.h"
#include "../Scene/scene.h"
#include <vector>

#define INFINITY_ 1e8 
class PolygonMesh : public Shape3D{

    std::vector<Triangle> triangles;
    std::vector<glm::vec3> vertices;
    PolygonMesh(std::vector<Triangle> triangles, glm::vec3 surfaceColor, glm::vec3 emissionColor,float transparency,float reflection)
    : triangles(triangles), Shape3D(getcenter(triangles), surfaceColor, emissionColor, transparency, reflection) {

    };

    void setvertices(){
        for(int i = 0; i < triangles.size(); i++){
            const Triangle& triangle = triangles[i];
            glm::vec3* v0 = NULL, *v1 = NULL, *v2 = NULL;
            for(int j = 0; j < vertices.size(); j++){
                if(vertices[j] == (glm::vec3) triangle.v0){
                    v0 = NULL; 
                }
                if(vertices[j] == (glm::vec3) triangle.v1){
                    v1 = NULL;
                }
                if(vertices[j] == (glm::vec3) triangle.v2){
                    v2 = NULL;
                }

            }
            if(v0 != NULL)  vertices.push_back(*v0);
            if(v1 != NULL)  vertices.push_back(*v1);
            if(v2 != NULL)  vertices.push_back(*v2);
        }
    }

    // friend bool operator== (glm::vec3& ours, glm::vec3& other) {
    //     if( (ours.x == other.v0 || ours.x == other.v1 || ours.x == other.v2) && 
    //         (ours.y == other.v0 || ours.y == other.v1 || ours.y == other.v2) &&
    //         (ours.z == other.v0 || ours.z == other.v1 || ours.z == other.v2))
    //         return false;
    //     return true;
    // }

    glm::vec3 getcenter(std::vector<Triangle> t_triangles){
        float x = 0, y = 0, z = 0;
        for(int i = 0; i < t_triangles.size(); i++){
            x += (t_triangles[i].v0.x  +  t_triangles[i].v1.x +  t_triangles[i].v2.x)/3  ;
            y += (t_triangles[i].v0.y  +  t_triangles[i].v1.y +  t_triangles[i].v2.y)/3  ;
            z += (t_triangles[i].v0.z  +  t_triangles[i].v1.z +  t_triangles[i].v2.z)/3  ;
        }
        return (glm::vec3(x/triangles.size(), y/triangles.size(), z/triangles.size()));
    }
    
   bool intersect_tri(const glm::vec3& start, const glm::vec3& dir, const Triangle triangle, glm::vec3* intersectpoint){
        glm::vec3 v0 = (glm::vec3) triangle.v0;
        glm::vec3 v1 = (glm::vec3) triangle.v1;
        glm::vec3 v2 = (glm::vec3) triangle.v2;
        glm::vec3 e1 = glm::vec3(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
        glm::vec3 e2 = glm::vec3(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);
        glm::vec3 b  = glm::vec3(start.x - v0.x, start.y - v0.y, start.z - v0.z);
        glm::vec3 d  = (glm::vec3) dir;
        glm::mat3 A( -d, e1, e2 );
        glm::vec3 x = glm::inverse( A ) * b;
       
        float t = x.x;
        float u = x.y;
        float v = x.z; 

       if (0 <= t && 0 <= u && 0 <= v && u + v <= 1){
            intersectpoint = new glm::vec3(x);
            return true;
       }
       return false;
    }

    bool trace(const glm::vec3 &orig, const glm::vec3 &dir, const std::vector<PolygonMesh*> polygons, glm::vec3* point_hit, Triangle* triangle_hit, PolygonMesh* polygon_hit) 
    { 
        float tNear= INFINITY_;
        bool found = false; 
     
        for(int k = 0; k < polygons.size(); k++){
            for (uint32_t i = 0; i < triangles.size(); ++i) { 
                float t = INFINITY_;
                glm::vec3* point_attempt = NULL;
                if (intersect_tri(orig, dir, triangles.at(i), point_attempt) && (t < tNear)) { 
                tNear = t; 
                    polygon_hit = (polygons[k]);
                    point_hit = point_attempt;
                    triangle_hit = &(triangles[i]);
                    found = true; 
                } 
            } 
        }
        if(!found) return false;
        else return true;

        // glm::vec3 surfaceColor(0,0,0)
        // normal = (glm::vec3)triangle_hit.normal;
        // bool inside = false;
        // if (glm::dot(normal,dir) > 0) { 
        //     normal = -normal; 
        //     inside = true; 
        // }
        // float bias = 1e-4;
    } 


    // static void render(const std::vector<PolygonMesh>& polygons){
    //     using glm::vec3;
    //     using glm::vec4;
    //     loadpolygons(polygons);
    //     for(int i=0; i<SCREEN_WIDTH; i++) {
    //         for(int j=0; j<SCREEN_HEIGHT; j++) {

    //             vec3 dir = vec4(i - SCREEN_WIDTH/2 - camera.position.x,
    //                             j - SCREEN_HEIGHT/2 - camera.position.y,
    //                             SCREEN_WIDTH/2 - camera.position.z)
    //             vec3* point_hit;
    //             Triangle* triangle_hit;
    //             PolygonMesh* polygon_hit;
    //             if (trace(camera.position, dir, polygons, point_hit, triangle_hit, polygon_hit)){
    //                 vec3 color = triangle_hit->color;
    //                 vec3 directlight = GetLightIntensity(lightSource, intersection, triangles);
    //                 color *= (directlight + INDIRECT_LIGHT) * intersection.triangle->gloss;
    //                 PutPixelSDL(screen, i, j, color);
    //             }

    //         }
    //         PrintProgress(i+1);
    //     }
    // }

    static void loadpolygons(std::vector<PolygonMesh>& polygons){
    using glm::vec3;
    using glm::vec4;

    // Defines colors:
    vec3 red(    0.75f, 0.15f, 0.15f );
    vec3 yellow( 0.75f, 0.75f, 0.15f );
    vec3 green(  0.15f, 0.75f, 0.15f );
    vec3 cyan(   0.15f, 0.75f, 0.75f );
    vec3 blue(   0.15f, 0.15f, 0.75f );
    vec3 purple( 0.75f, 0.15f, 0.75f );
    vec3 white(  0.75f, 0.75f, 0.75f );

    vec3 lowGloss(0.7, 0.7, 0.7);
    vec3 normalGloss(1, 1, 1);
    vec3 someGloss(1.2, 1.2, 1.2);
    vec3 moreGloss(1.5, 1.5, 1.5);
    vec3 highGloss(2, 2, 2);
    vec3 superGloss(5, 5, 5);

    // ---------------------------------------------------------------------------
    // Room

    std::vector<Triangle> triangles;
    float L = 555;			// Length of Cornell Box side.

    vec4 A(L,0,0,1);
    vec4 B(0,0,0,1);
    vec4 C(L,0,L,1);
    vec4 D(0,0,L,1);

    vec4 E(L,L,0,1);
    vec4 F(0,L,0,1);
    vec4 G(L,L,L,1);
    vec4 H(0,L,L,1);

    // Floor:
    triangles.push_back( Triangle( C, B, A, green , moreGloss) );
    triangles.push_back( Triangle( C, D, B, green , moreGloss) );

    // Left wall
    triangles.push_back( Triangle( A, E, C, purple, highGloss ) );
    triangles.push_back( Triangle( C, E, G, purple, highGloss ) );

    // Right wall
    triangles.push_back( Triangle( F, B, D, yellow, lowGloss ) );
    triangles.push_back( Triangle( H, F, D, yellow, lowGloss ) );

    // Ceiling
    triangles.push_back( Triangle( E, F, G, cyan , normalGloss) );
    triangles.push_back( Triangle( F, H, G, cyan , normalGloss) );

    // Back wall
    triangles.push_back( Triangle( G, D, C, white, someGloss ) );
    triangles.push_back( Triangle( G, H, D, white, someGloss ) );
    polygons.push_back(PolygonMesh(triangles, vec3(23,12,55), vec3(30,100,4), 5,5));
    // ---------------------------------------------------------------------------
    // Short block

    A = vec4(290,0,114,1);
    B = vec4(130,0, 65,1);
    C = vec4(240,0,272,1);
    D = vec4( 82,0,225,1);

    E = vec4(290,165,114,1);
    F = vec4(130,165, 65,1);
    G = vec4(240,165,272,1);
    H = vec4( 82,165,225,1);
    std::vector<Triangle> triangles2;
    // Front
    triangles2.push_back( Triangle(E,B,A,red, someGloss) );
    triangles2.push_back( Triangle(E,F,B,red, someGloss) );

    // Front
    triangles2.push_back( Triangle(F,D,B,red, someGloss) );
    triangles2.push_back( Triangle(F,H,D,red, someGloss) );

    // BACK
    triangles2.push_back( Triangle(H,C,D,red, someGloss) );
    triangles2.push_back( Triangle(H,G,C,red, someGloss) );

    // LEFT
    triangles2.push_back( Triangle(G,E,C,red, someGloss) );
    triangles2.push_back( Triangle(E,A,C,red, someGloss) );

    // TOP
    triangles2.push_back( Triangle(G,F,E,red, superGloss) );
    triangles2.push_back( Triangle(G,H,F,red, superGloss) );
    polygons.push_back(PolygonMesh(triangles2, vec3(23,12,55), vec3(30,100,4), 5,5));
    // ---------------------------------------------------------------------------
    // Tall block

    A = vec4(423,0,247,1);
    B = vec4(265,0,296,1);
    C = vec4(472,0,406,1);
    D = vec4(314,0,456,1);

    E = vec4(423,330,247,1);
    F = vec4(265,330,296,1);
    G = vec4(472,330,406,1);
    H = vec4(314,330,456,1);
    std::vector<Triangle> triangles3;
    // Front
    triangles3.push_back( Triangle(E,B,A,blue, highGloss) );
    triangles3.push_back( Triangle(E,F,B,blue, highGloss) );

    // Front
    triangles3.push_back( Triangle(F,D,B,blue, highGloss) );
    triangles3.push_back( Triangle(F,H,D,blue, highGloss) );

    // BACK
    triangles3.push_back( Triangle(H,C,D,blue, highGloss) );
    triangles3.push_back( Triangle(H,G,C,blue, highGloss) );

    // LEFT
    triangles3.push_back( Triangle(G,E,C,blue, highGloss) );
    triangles3.push_back( Triangle(E,A,C,blue, highGloss) );

    // TOP
    triangles3.push_back( Triangle(G,F,E,blue, superGloss) );
    triangles3.push_back( Triangle(G,H,F,blue, superGloss) );
    polygons.push_back(PolygonMesh(triangles3, vec3(23,12,55), vec3(30,100,4), 5,5));

    // ----------------------------------------------
    // Scale to the volume [-1,1]^3
    for(int k = 0; k < polygons.size(); k ++){
        std::vector<Triangle> triangles = polygons[k].triangles;
        for( size_t i=0; i<triangles.size(); ++i )
        {
            triangles[i].v0 *= 2/L;
            triangles[i].v1 *= 2/L;
            triangles[i].v2 *= 2/L;

            triangles[i].v0 -= vec4(1,1,1,1);
            triangles[i].v1 -= vec4(1,1,1,1);
            triangles[i].v2 -= vec4(1,1,1,1);

            triangles[i].v0.x *= -1;
            triangles[i].v1.x *= -1;
            triangles[i].v2.x *= -1;

            triangles[i].v0.y *= -1;
            triangles[i].v1.y *= -1;
            triangles[i].v2.y *= -1;

            triangles[i].v0.w = 1.0;
            triangles[i].v1.w = 1.0;
            triangles[i].v2.w = 1.0;
        }
    }
    }
};



#endif