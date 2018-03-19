#ifndef RASTERISER_H
#define RASTERISER_H

#include <glm/glm.hpp>
#include <glm/gtx/normal.hpp>
#include "../light/lightsource.h"
#include "../scene/camera.h"
#include "../shapes/triangle.h"
#include <vector>
using glm::vec3;
using glm::vec4;
using glm::mat4;
using namespace std;

class Rasteriser {
public:
    Pixel depthBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
    Pixel shadowBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
    glm::vec3 frameBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];

    Rasteriser(int a) {
           cout << "here\n";
    };


    glm::mat4 ModelViewMatrix(vec3 eye, vec3 center, vec3 up) {
        vec3 zaxis = glm::normalize(eye - center);
        vec3 xaxis = glm::normalize(glm::cross(up, zaxis));
        vec3 yaxis = glm::cross(zaxis, xaxis);
        glm::mat4 minv(1);
        glm::mat4 tr(1);
        for (int i=0; i<3; i++) {
            minv[0][i] = xaxis[i];
            minv[1][i] = yaxis[i];
            minv[2][i] = zaxis[i];
            tr[i][3] = -center[i];
        }

        return minv*tr;   
    }
    glm::vec3 GetBarycentric(glm::vec3 aV1, glm::vec3 aV2, glm::vec3 aV3, glm::vec3 aP)
    {
        glm::vec3 a = aV2 - aV3, b = aV1 - aV3, c = aP - aV3;
        float aLen = a.x * a.x + a.y * a.y + a.z * a.z;
        float bLen = b.x * b.x + b.y * b.y + b.z * b.z;
        float ab = a.x * b.x + a.y * b.y + a.z * b.z;
        float ca = a.x * c.x + a.y * c.y + a.z * c.z;
        float cb = b.x * c.x + b.y * c.y + b.z * c.z;
        float d = aLen * bLen - ab * ab;
        glm::vec3 B = vec3((aLen * cb - ab * ca), (bLen * ca - ab * cb), 1.f)/d;
        B.z = 1.0f - B.x - B.y;
        return B;
    }
    glm::mat4 ViewPort(int x, int y, int w, int h, int depth) {
        glm::mat4 m(1); 
        m[0][3] = x+w/2.f;
        m[1][3] = y+h/2.f;
        m[2][3] = depth/2.f;

        m[0][0] = w/2.f;
        m[1][1] = h/2.f;
        m[2][2] = depth/2.f;
        return m;
    }

    glm::mat4 PerspectiveProjectionMatrix(float c) { // c = distance of camera from origin

        glm::mat4 projection(  vec4(   1.f,       0,       0,      0 ),
                               vec4(   0,       1.f,       0,      0 ),
                               vec4(   0,       0,       1.f,    -(1.f/c)),
                               vec4(   0,       0,       0,      1.f ));
      
        return projection;          
    }  


    // vec4 vertexToScreenSpace(glm::mat4 viewPort, glm::mat4 projection, glm::mat4 modelView, Triangle* triangle, int nthSide){
    //     vec4 vertex;
    //     if(nthSide == 0)
    //         vertex = triangle->v0;
    //     else if(nthSide == 1)
    //         vertex =  triangle->v1;
    //     else if(nthSide == 2)
    //         vertex = triangle->v2;
    
    //     for(int i = 0; i < 3; i++){
    //         varying_uv[nthSide][i] = vertex[i];
    //     }
        
    //     vec4 gl_Vertex = viewPort*projection*modelView*vertex;
    //     for(int i = 0; i < 3; i++){
    //         varying_uv[nthSide][i] = gl_Vertex[nthSide]/gl_Vertex[3];
    //     }

    //     return gl_Vertex;
    // }

    void ProcessPolygonLines( screen *screen, LightSource* lightSource, const vec3& origin, const vector<vec4>& vertices, const vec3 color, Shape2D* shape, bool lightM = false) {
        int V = vertices.size();
        vector<Pixel> vertexPixels( V );
        for( int i=0; i<V; i++ ) {
            VertexShader(vertices[i], origin, vertexPixels[i], shape);
        }

        vector<Pixel> leftPixels;
        vector<Pixel> rightPixels;
        ProcessPolygonRows(origin, vertexPixels, leftPixels, rightPixels );
        for (size_t i = 0; i < leftPixels.size(); i++) {
            ProcessPolygonLine(screen, lightSource, origin, leftPixels[i], rightPixels[i], color, lightM);
        }
    }
    // @: Takes 4D position of a vertex v, computes 2D image position, stores it inside 2D pixel vector p = (x,y) and assigns corresponding 2d shape
    static void VertexShader(const vec4& v, const vec3& origin, Pixel& p, Shape2D* shape) {
        vec4 v_prime = v - vec4(origin.x, origin.y, origin.z, 1);
        p.x = focal_length * (v_prime.x / v_prime.z) + (SCREEN_WIDTH / 2);
        p.y = focal_length * (v_prime.y / v_prime.z) + (SCREEN_HEIGHT / 2);
        p.zinv = 1 / v_prime.z;
        p.pos3d = vec3(v);
        p.shape = shape;
    }
    // @: Computes 2 vectors 'leftPixels' & 'rightPixels' storing the min & max x-value respectively, for each horizontal row the shape occupies
    void ProcessPolygonRows(const vec3& origin, const vector<Pixel>& vertexPixels, vector<Pixel>& leftPixels, vector<Pixel>& rightPixels) {
        int minY = vertexPixels[0].y;
        int maxY = vertexPixels[0].y;
        
        /* Find max and min y-value of the polygon and compute the number of rows it occupies,
        where the amount of rows in a polygon is the amount of vertical space it occupies. */
        for (size_t i = 0; i < vertexPixels.size(); i++) {
            if (minY > vertexPixels[i].y) {
                minY = vertexPixels[i].y;
            }
            if (maxY < vertexPixels[i].y) {
                maxY = vertexPixels[i].y;
            }
        }

        /* Left Pixels & Right Pixels have size equal to number of rows in the polygon
        - Left pixels have x value equal to min x value of the row 
        - Right pixels have x value equal to max x value of the row  */

        // Initialise the right & left pixel vectors
        int rows = maxY - minY + 1;
        leftPixels = vector<Pixel>(rows);
        rightPixels = vector<Pixel>(rows);
        for (int i = 0; i < rows; i++) {
            //Initialize the x-coordinates in leftPixels to some really large value
            leftPixels[i].x     = +numeric_limits<int>::max();
            //Initialize the x-coordinates in rightPixels to some really small value
            rightPixels[i].x    = -numeric_limits<int>::max();

            //Resize leftPixels and rightPixels so that they have an element for each row
            leftPixels[i].y     = minY + i;
            rightPixels[i].y    = minY + i;
        }

        /* Compute the right & left pixel vectors */

        // - Loop through all edges of the polygon
        for (size_t i = 0; i < vertexPixels.size(); i++) {
            int dx = abs(vertexPixels[i].x - vertexPixels[(i + 1) % vertexPixels.size()].x);
            int dy = abs(vertexPixels[i].y - vertexPixels[(i + 1) % vertexPixels.size()].y);
            int pixels = max(dx, dy) + 1;

            // Use linear interpolation to find the x-coordinate for each row it occupies.
            vector<Pixel>line = vector<Pixel> (pixels);
            Pixel::Interpolate(origin, vertexPixels[i], vertexPixels[(i + 1) % vertexPixels.size()], line);
            
            //Update the corresponding values in rightPixels and leftPixels.
            for (int r = 0; r < rows; r++) {
                for (size_t l = 0; l < line.size(); l++) {

                    if (line[l].y == minY + r) {
                        if (line[l].x < leftPixels[r].x) {
                            leftPixels[r].x = line[l].x;
                            leftPixels[r].zinv = line[l].zinv;
                            leftPixels[r].shape = line[l].shape;
                        }

                        if (line[l].x > rightPixels[r].x) {
                            rightPixels[r].x = line[l].x;
                            rightPixels[r].zinv = line[l].zinv;
                            rightPixels[r].shape = line[l].shape;
                        }
                    }
                }
            }
        }
    }
    void ProcessPolygonLine(screen* screen,  LightSource* lightSource, const vec3& origin, const Pixel& a, const Pixel& b, vec3 color, bool lightM) {
        int dx = abs(a.x - b.x);
        int dy = abs(a.y - b.y);
        int pixels = max(dx, dy) + 1;

        vector<Pixel> line(pixels);
        Pixel::Interpolate(origin, a, b, line);
        if(lightM){
            for (size_t i = 0; i < line.size(); i++) {

                //Clipping
                if( line[i].x > 0 && 
                    line[i].y > 0 && 
                    line[i].x < SCREEN_WIDTH && 
                    line[i].y < SCREEN_HEIGHT){

                    if (line[i].zinv > shadowBuffer[line[i].x][line[i].y].zinv) {
                        shadowBuffer[line[i].x][line[i].y] = line[i];
                    }
                }
            }
        }
        else{
            for (size_t i = 0; i < line.size(); i++) {

                //Clipping
                if( line[i].x > 0 && 
                    line[i].y > 0 && 
                    line[i].x < SCREEN_WIDTH && 
                    line[i].y < SCREEN_HEIGHT){

                    if (line[i].zinv > depthBuffer[line[i].x][line[i].y].zinv) {
                        depthBuffer[line[i].x][line[i].y] = line[i];
                    }
                }
            }   
        }
    }

    void ProcessColor(){
        memset(&frameBuffer, 0, sizeof(frameBuffer));
        for(int i = 0; i < SCREEN_HEIGHT; i++){
            for(int j = 0; j < SCREEN_WIDTH; j++){
                    frameBuffer[i][j] = vec3(0,0,0);
                    for(int c = max(i-1, 0); c < min(i+2,SCREEN_HEIGHT); c++){
                        for(int d = max(j-1, 0); d < min(j+1,SCREEN_WIDTH); d++){
                            if(depthBuffer[c][d].shape != nullptr)
                                frameBuffer[i][j] += depthBuffer[c][d].shape->color;
                        }
                    }
                    frameBuffer[i][j] /= 8.f;
                
            }
        }
    }

    void ProcessLight(LightSource* lightSource, glm::mat4 lightMatrix, glm::mat4 cameraMatrix){
        for(int i = 0; i < SCREEN_HEIGHT; i++){
            for(int j = 0; j < SCREEN_WIDTH; j++){
                vec3 color = frameBuffer[i][j];
                if(depthBuffer[i][j].shape == nullptr) {
                    frameBuffer[i][j] = color;
                }
                else {

                    Pixel lightPix = shadowBuffer[i][j];
                    Pixel cameraPix = depthBuffer[i][j];
                    vec4 lightUV = vec4(lightPix.x, lightPix.y, lightPix.zinv, 1);
                    vec4 cameraUV = vec4(cameraPix.x, cameraPix.y, cameraPix.zinv, 1);

                    vec4 camtolight = lightMatrix * glm::inverse(cameraMatrix) * cameraUV;
                    glm::vec2 newUV(camtolight.x, camtolight.y);

                    if(depthBuffer[(int)newUV.x][(int)newUV.y].zinv < shadowBuffer[(int)lightUV.x][(int)lightUV.y].zinv){
                            frameBuffer[i][j] = vec3(0,0,0);
                    }
                    else{
                        vec3 dis = (vec3)(lightSource->position) - depthBuffer[i][j].pos3d;
                        float r = glm::length(dis);
                        r = 4.0 * 3.1415926 * r * r;

                        float result =  dis.x *  depthBuffer[i][j].shape->ComputeNormal().x + 
                                        dis.y *  depthBuffer[i][j].shape->ComputeNormal().y + 
                                        dis.z *  depthBuffer[i][j].shape->ComputeNormal().z;

                        vec3 light_area = result / r * lightSource->power;
                        light_area = (lightSource->indirect_light + light_area);
                        frameBuffer[i][j] = color * light_area; 
                    }
                    
                }
            }
        }
    }

    void RenderFrameBuffer(screen* screen){
        for(int i = 0; i < SCREEN_HEIGHT; i++){
            for(int j = 0; j < SCREEN_WIDTH; j++){
                PutPixelSDL(screen, i, j, frameBuffer[i][j]);
            }
        }
    }

    void Draw(screen* screen, LightSource* lightSource, Camera& camera, std::vector<Shape2D*>& shapes){
        vec4 light_pos = lightSource->position;
        vec4 light_dir = lightSource->direction;
        vec4 camera_pos = camera.position;
	    vec4 camera_dir = camera.getDirection(0,0);
        glm::mat4 lightModelView        = ModelViewMatrix((vec3)light_dir,(vec3) light_pos, vec3(0.f,1.f,0.f));
        glm::mat4 lightViewPort         = ViewPort(SCREEN_WIDTH/8, SCREEN_HEIGHT/8, SCREEN_WIDTH*3/4, SCREEN_HEIGHT*3/4, 255);
        glm::mat4 light_projection      = PerspectiveProjectionMatrix(-1.f);
        glm::mat4 cam_ModelView        = ModelViewMatrix((vec3)camera_dir,(vec3) camera_pos, vec3(0.f,1.f,0.f));
        glm::mat4 cam_ViewPort         = ViewPort(SCREEN_WIDTH/8, SCREEN_HEIGHT/8, SCREEN_WIDTH*3/4, SCREEN_HEIGHT*3/4, 255);
        glm::mat4 cam_projection        = PerspectiveProjectionMatrix(-1.f/glm::length(camera_pos-(camera_pos+camera_dir)));
        glm::mat4 lightMatrix = lightViewPort*light_projection*lightModelView;
        glm::mat4 cameraMatrix = cam_ViewPort*cam_projection*cam_ModelView;
        memset(&(shadowBuffer), 0, sizeof(shadowBuffer));
        memset(&(depthBuffer), 0, sizeof(depthBuffer));

        for ( size_t i = 0; i < shapes.size(); i++) {
            std::vector<vec4> verticies = shapes[i]->verticies();
            ProcessPolygonLines(screen, lightSource, (vec3) light_pos, verticies, shapes[i]->color, shapes[i], true);
        }

        for ( size_t i = 0; i < shapes.size(); i++) {
            std::vector<vec4> verticies = shapes[i]->verticies();
            ProcessPolygonLines(screen, lightSource, (vec3) camera_pos, verticies, shapes[i]->color, shapes[i], false);
        }
        cout << "here\n";
        ProcessColor();
        ProcessLight(lightSource,lightMatrix,cameraMatrix);
        RenderFrameBuffer(screen);
        
    }

};


#endif 