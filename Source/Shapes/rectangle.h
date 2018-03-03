// #ifndef RECTANGLE_H
// #define RECTANGLE_H
// #include "shape2D.h"
// #include <vector>
// #include <initializer_list>
// #include <glm/glm.hpp>
// #include <glm/gtx/normal.hpp>
// #include "../Light/ray.h"
// #include "../Materials/material.h"
// #include "triangle.h"
// using namespace std;
// using glm::vec3;
// using glm::vec4;

// class Rectangle : public Shape2D
// {
// public:
//     vec4 bottom_left;
//     vec4 bottom_right;
//     vec4 top_left;
//     vec4 top_right;
    
//     Rectangle(  vec4 t_bottom_left, 
//                 vec4 t_bottom_right, 
//                 vec4 t_top_left, 
//                 vec4 t_top_right, 
//                 vec3 color, const std::initializer_list<Material*>& materials = std::initializer_list<Material*>()) 
//     : Shape2D(color, materials), bottom_left(toworld(t_bottom_left, false)), bottom_right(toworld((t_bottom_right, false)), top_left(toworld((t_top_left, true)), top_right(toworld(t_top_right, true)) {

//     };

//     void thing(){
//         vec4 A(2.f*L,0,0,1);
//         vec4 B(0,0,0,1);
//         vec4 C(2.f*L,0,L,1);
//         vec4 D(0,0,L,1);
//     }
//     vec4 toworld(glm::vec4 cam_intersect, bool isBottomrect) {
//         if(isBottomrect){
//             float u = cam_intersect[1], v = cam_intersect[2];
//             vec4 e1 = bottom_right - bottom_left;
//             vec4 e2 = top_left - bottom_left;
//             return bottom_left + u * e1 + v * e2;
//         }
//         else{
//             float u = cam_intersect[1], v = cam_intersect[2];
//             vec4 e2 = bottom_right - top_right;
//             vec4 e3 = top_left - top_right;
//             return top_right + u * e2 + v * e3;
//         }
//     }
//     virtual bool intersect(Ray& ray, vec3 dir, vec4& intersectionpoint) override {

//         vec3 e1 = vec3(bottom_right.x - bottom_left.x, bottom_right.y - bottom_left.y, bottom_right.z - bottom_left.z);
//         vec3 e2 = vec3(top_left.x - bottom_left.x, top_left.y - bottom_left.y, top_left.z - bottom_left.z);
//         vec3 e3 = vec3(top_right.x - top_left.x, top_right.y - top_left.y, top_right.z - top_left.z);
//         vec3 b1 = vec3(ray.position.x - bottom_left.x, ray.position.y - bottom_left.y, ray.position.z - bottom_left.z);
//         vec3 b2 = vec3(ray.position.x - top_right.x, ray.position.y - top_right.y, ray.position.z - top_right.z);
//         glm::mat3 A1( -dir, e1, e2 );
//         glm::mat3 A2( -dir, e2, e3 );
//         vec3 x1 = glm::inverse( A1 ) * b1;
//         vec3 x2 = glm::inverse( A2 ) * b2;
//         float t1 = x1.x;
//         float u1 = x1.y;
//         float vv1 = x1.z; 
//         float t2 = x2.x;
//         float u2 = x2.y;
//         float vv2 = x2.z;

//         if (0 <= t1 && 0 <= u1 && 0 <= vv1 && u1 + vv1 <= 1) { 
//             intersectionpoint = toworld(vec4(t1,u1,vv1,1), false);
//             return true;
//         } 
//         else if  (0 <= t2 && 0 <= u2 && 0 <= vv2 && u2 + vv2 <= 1){
//             intersectionpoint = toworld(vec4(t2,u2,vv2,1), true);
//             return true;
//         }
//         return false;
//     }
//     virtual vec4 toworldcoordinates(glm::vec4 cam_intersect){

//     };
//     virtual glm::vec3 getcolor(Intersection& intersection, const Ray& primary_ray, const std::vector<Shape2D*>& shapes, LightSource* lightSource) override{
//         vector<vec3> colors;
//         for(int a = 0; a < materials.size(); a++){
//             colors.push_back(materials[a]->material_color(intersection, primary_ray, shapes, lightSource));
//         }
//         if(colors.size() == 0)
//             return vec3(0,0,0);

//         printf("Color\n");
//         if(colors.size() == 1)
//             return colors[0];

//         printf("Color\n");
//         vec3 t_color = glm::mix(colors[0], colors[1], 0.5f);    
//         for(int a = 2; a < colors.size(); a++){
//             t_color = glm::mix(t_color, colors[a], 0.5f);
//         }
//         return t_color;
//     }
 
//     virtual glm::vec3 getnormal(Intersection& intersection) override{
//         vec3 a =  (vec3) glm::normalize(glm::triangleNormal((vec3) bottom_left, (vec3) top_left, (vec3) bottom_right));
//         vec3 b = -a;
//         if(glm::dot(a, (vec3) glm::normalize(intersection.direction)) <= 0)
//             return a;
//         else
//             return b;
//     }
//     virtual vec3 minPosition() override {
//         vec3 minPos = (vec3)bottom_left;
//         for (int i = 0; i < 3; i++) {
//             if ( bottom_right[i] < minPos[i] ) minPos[i] = bottom_right[i];
//         }
//         for (int i = 0; i < 3; i++) {
//             if ( top_left[i] < minPos[i] ) minPos[i] = top_left[i];
//         }
//         for (int i = 0; i < 3; i++) {
//             if ( top_right[i] < minPos[i] ) minPos[i] = top_right[i];
//         }
//         return minPos;
//     };
//     virtual vec3 maxPosition() override {
//         vec3 minPos = (vec3)bottom_left;
//         for (int i = 0; i < 3; i++) {
//             if ( bottom_right[i] > minPos[i] ) minPos[i] = bottom_right[i];
//         }
//         for (int i = 0; i < 3; i++) {
//             if ( top_left[i] > minPos[i] ) minPos[i] = top_left[i];
//         }
//         for (int i = 0; i < 3; i++) {
//             if ( top_right[i] > minPos[i] ) minPos[i] = top_right[i];
//         }
//         return minPos;
//     };
//     virtual vec3 midpoint() override {
//         vec3 point = vec3(0, 0, 0);
//         point.x = (bottom_right.x + bottom_left.x + top_left.x + top_right.z) / 4.0;
//         point.y = (bottom_right.y + bottom_left.y + top_left.y + top_right.z) / 4.0;
//         point.z = (bottom_right.z + bottom_left.z + top_left.z + top_right.z) / 4.0;
//         return point;
//     };
//     virtual bool isEqual(const Shape2D& other) const override {
//         Rectangle rect = static_cast<const Rectangle&>(other);
//         if( (bottom_left == rect.bottom_left && bottom_right == rect.bottom_right && top_left == rect.top_left) ||
//             (bottom_left == rect.bottom_left && bottom_right == rect.top_left && top_left == rect.bottom_right) ||
//             (bottom_left == rect.bottom_right && bottom_right == rect.top_left && top_left == rect.bottom_left) ||
//             (bottom_left == rect.bottom_right && bottom_right == rect.bottom_left && top_left == rect.top_left) ||
//             (bottom_left == rect.top_left && bottom_right == rect.bottom_left && top_left == rect.bottom_right) ||
//             (bottom_left == rect.top_left && bottom_right == rect.bottom_right && top_left == rect.bottom_left)){
//             return true;
//         }
//         return false;
//     };
// };



// #endif 
