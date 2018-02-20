#include "intersection.h"
#include "ray.h"

glm::vec3 Intersection::compute_color(Ray& primary_ray, const std::vector<Shape2D*>& shapes){

    vec3 color          = shape2D->getcolor(*(this), primary_ray, shapes);
    // vec3 directlight    = primary_ray.GetDirectLight(*(this), shapes);
    // vec3 indirectlight  = primary_ray.GetIndirectLight(); 
    // color *= ((directlight + indirectlight) * (vec3) shape2D->gloss);
    return color;  
}  