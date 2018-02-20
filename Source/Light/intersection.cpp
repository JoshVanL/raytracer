#include "intersection.h"
#include "ray.h"

glm::vec3 Intersection::compute_color(const Ray& primary_ray, const std::vector<Shape2D*>& shapes){
 
    vec3 color          = shape2D->getcolor(*(this), primary_ray, shapes);
    return color;  
}   



