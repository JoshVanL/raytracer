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
//     Triangle* tri_A;
//     Triangle* tri_B;
//     Rectangle(  vec4 t_bottom_left, 
//                 vec4 t_bottom_right, 
//                 vec4 t_top_left, 
//                 vec4 t_top_right, 
//                 vec3 color, const std::initializer_list<Material*>& materials = std::initializer_list<Material*>()) 
//     : Shape2D(color, materials), 
//         tri_A(new Triangle( t_top_right, t_bottom_left, t_top_left, color ,  materials)), 
//         tri_B(new Triangle( t_top_right, t_bottom_right, t_bottom_left, color ,  materials )) {

//     };



//     virtual bool intersect(Ray& ray, vec3 dir, vec4& intersectionpoint) override {
//         return (tri_A->intersect(ray, dir, intersectionpoint) || tri_B->intersect(ray, dir, intersectionpoint));
//     }
   
//     virtual glm::vec3 getcolor(Intersection& intersection, const Ray& primary_ray, const std::vector<Shape2D*>& shapes, LightSource* lightSource) override{
//         if(intersection.shape2D == tri_A)
            
//         vec4 point = intersection.position;
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
