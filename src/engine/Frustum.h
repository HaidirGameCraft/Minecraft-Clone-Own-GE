#pragma once

#include <engine/Vector.h>
#include <engine/Camera.h>
#include <engine/Collider.h>
#include <engine/Plane.h>

class FrustumPlane {
public:
    Vec3 normal;
    float distance;
};

class Frustum {
public:
    FrustumPlane planes[6];
    Frustum();
    ~Frustum();
    void CalculateFrustum(Camera& camera);
    bool IsPointInsideFrustum(Vec3 point);
    bool IsBoxInsideFrustum(BoxCollider* collider);

    enum FrustumIndex {
        F_LEFT,
        F_RIGHT,
        F_TOP,
        F_BOTTOM,
        F_NEAR,
        F_FAR
    };
};