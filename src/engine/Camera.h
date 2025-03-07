#pragma once
#include <engine/Transform.h>

class Frustum;

class Camera {
public:
    Transform transform;
    Frustum* frustum;
};

class PerspectiveCamera : public Camera
{
public:
    
    float fov;
    float aspect;
    float near;
    float far;
    PerspectiveCamera(float fov, float aspect, float near, float far);
    void UpdateMatrix();
    void Create_Projection();
    
};