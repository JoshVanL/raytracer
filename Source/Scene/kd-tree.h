#ifndef KD_TREE_H
#define KD_TREE_H
#include "SDL.h"
#include "../Light/intersection.h"
#include "../Light/ray.h"
#include "../Shapes/shape2D.h"

class BoundingBox {
    public:
        vec3 minPos;
        vec3 maxPos;

        BoundingBox() {
            minPos = vec3{0, 0, 0};
            maxPos = vec3{0, 0, 0};
        }

        void get_bounding_box(Shape2D* shape) {
            minPos = shape->minPosition();
            maxPos = shape->maxPosition();
        }

        int longest_axis() {
            int axis = 0;
            if ((maxPos[axis] - minPos[axis]) < (maxPos[1] - minPos[1])) {
                axis = 1;
            }
            if ((maxPos[axis] - minPos[axis]) < (maxPos[2] - minPos[2])) {
                axis = 2;
            }
            return axis;
        }

        bool hit(Camera &camera, vec4 dir) {
            vec3 dirfrac;
            dirfrac.x = 1.0f / dir.x;
            dirfrac.y = 1.0f / dir.y;
            dirfrac.z = 1.0f / dir.z;
            float t1 = (minPos.x - camera.position.x)*dirfrac.x;
            float t2 = (maxPos.x - camera.position.x)*dirfrac.x;
            float t3 = (minPos.y - camera.position.y)*dirfrac.y;
            float t4 = (maxPos.y - camera.position.y)*dirfrac.y;
            float t5 = (minPos.z - camera.position.z)*dirfrac.z;
            float t6 = (maxPos.z - camera.position.z)*dirfrac.z;

            float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
            float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

            // if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
            if (tmax < 0 ) {
                return false;
            }
            // if tmin > tmax, ray doesn't intersect AABB
            if (tmin > tmax) {
                return false;
            }

            return true;
        }

        void expand(BoundingBox* bbox) {
            for (int i = 0; i < 3; i++) {
                if (minPos[i] > bbox->minPos[i]) {
                    minPos[i] = bbox->minPos[i];
                }
                if (maxPos[i] < bbox->maxPos[i]) {
                    maxPos[i] = bbox->maxPos[i];
                }
            }
        }
};

class KDNode {
    public:
        BoundingBox* bbox;
        KDNode* left;
        KDNode* right;
        vector<Shape2D*> shapes;

        KDNode() {
            bbox = new BoundingBox();
            left = NULL;
            right = NULL;
            shapes = vector<Shape2D*>();
        }


        KDNode* buildTree(vector<Shape2D*>& shapes, int depth) const {
            KDNode* node = new KDNode();
            node->shapes = shapes;

            if (shapes.size() == 0) {
                return node;
            }

            if (shapes.size() == 1) {
                node->bbox->get_bounding_box(shapes[0]);
                node->left = new KDNode();
                node->right = new KDNode();
                return node;
            }

            node->bbox->get_bounding_box(shapes[0]);

            for (int i = 1; i< shapes.size(); i++) {
                BoundingBox* bbx = new BoundingBox();
                bbx->get_bounding_box(shapes[i]);
                node->bbox->expand(bbx);
            }

            vec3 midpoint = vec3(0, 0, 0);
            for (int i = 0; i < shapes.size(); i++) {
                midpoint += shapes[i]->midpoint();
            }
            midpoint.x = midpoint.x / shapes.size();
            midpoint.y = midpoint.y / shapes.size();
            midpoint.z = midpoint.z / shapes.size();

            vector<Shape2D*> lefts;
            vector<Shape2D*> rights;
            int axis = node->bbox->longest_axis();
            for (int i = 0; i < shapes.size(); i++) {
                switch (axis) {
                    case 0:
                        midpoint.x >= shapes[i]->midpoint().x ? rights.push_back(shapes[i]) : lefts.push_back(shapes[i]);
                        break;
                    case 1:
                        midpoint.y >= shapes[i]->midpoint().y ? rights.push_back(shapes[i]) : lefts.push_back(shapes[i]);
                        break;
                    case 2:
                        midpoint.z >= shapes[i]->midpoint().z ? rights.push_back(shapes[i]) : lefts.push_back(shapes[i]);
                        break;
                }
            }

            if (lefts.size() == 0 && rights.size()  > 0) lefts  = rights;
            if (lefts.size()  > 0 && rights.size() == 0) rights = lefts;

            // If matches > 50% stop subdividing
            int matches= 0;
            for (int i = 0; i < lefts.size(); i++) {
                for (int j = 0; j < rights.size(); j++) {
                    if (lefts[i] == rights[j]) {
                        matches++;
                    }
                }
            }

            if ((float)matches / lefts.size() < 0.5 && (float)matches / rights.size() < 0.5) {
                // recurse down left and right side
                node->left  = buildTree(lefts, depth + 1);
                node->right = buildTree(rights, depth + 1);
            } else {
                node->left = new KDNode();
                node->right = new KDNode();
            }

            return node;
        }


        bool hit(KDNode* node, Camera& camera, vec4 dir, Intersection &intersection) {
            if (bbox->hit(camera, dir)) {
                if (node->left->shapes.size() > 0 || node->right->shapes.size() > 0) {
                    bool hitleft = hit(node->left, camera, dir, intersection);
                    bool hitright = hit(node->right, camera, dir, intersection);

                    return hitleft || hitright;

                    // Reached leaf
                } else {
                    if(camera.primary_ray.ClosestIntersection(dir, shapes, intersection)) {
                        return true;
                    }
                }
            }

            return false;
        }
};

#endif
