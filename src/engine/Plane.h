#pragma once
#include <engine/Vector.h>

class Plane {
public:
    Vec3 normal;
    float d;
    Plane();
    Plane(Vec3 normals);
};