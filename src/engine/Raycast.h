#pragma once

#include <engine/Vector.h>
#include <engine/Collider.h>
#include <vector>

template <typename T>
class RayIntersect {
public:
    Vec3 point;
    float t;
    T* object;
};

class Raycast {
public:
    Vec3 origin;
    Vec3 direction;
    Raycast(Vec3 origin, Vec3 dir);
    
    std::vector<RayIntersect<BoxCollider>> IntersectBoxes(std::vector<BoxCollider>& colliders, float max_distance);
    RayIntersect<BoxCollider> IntersectBox(BoxCollider& colliders, float max_distance);
    bool CalculateBoxIntersect(BoxCollider& collider, float max_distance);
};