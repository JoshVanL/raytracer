#ifndef GLOSS_H
#define GLOSS_H
#include <glm/glm.hpp>
using glm::vec3;

enum GlossType {
    LOWGLOSS,
    NORMALGLOSS,
    SOMEGLOSS,
    MOREGLOSS,
    HIGHGLOSS,
    SUPERGLOSS
};
class Gloss {
public:
    vec3 gloss;
    Gloss(GlossType gt): gloss(convertGlossType(gt)) {

    };

    vec3 convertGlossType(GlossType gt){
        if      (gt == LOWGLOSS)    return vec3(0.7, 0.7, 0.7);
        else if (gt == NORMALGLOSS) return vec3(1, 1, 1);
        else if (gt == SOMEGLOSS)   return vec3(1.2, 1.2, 1.2);
        else if (gt == MOREGLOSS)   return vec3(1.5, 1.5, 1.5);
        else if (gt == HIGHGLOSS)   return vec3(2, 2, 2);
        else if (gt == SUPERGLOSS)  return vec3(5, 5, 5);
    }
};
#endif