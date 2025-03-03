#pragma once
#include <engine/Transform.h>


class Camera {
public:
    Transform transform;
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