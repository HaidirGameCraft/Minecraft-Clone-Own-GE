#pragma once

#include <engine/Vector.h>
#include <engine/Mat4.h>

class Transform
{
public:
    Vec3 position;
    Vec3 rotation;
    Vec3 scale;
    Mat4 local_matrix;
    Mat4 world_matrix;
    Mat4 projection_matrix;

    Vec3 forward;
    Vec3 back;
    Vec3 right;
    Vec3 left;
    Vec3 up;
    Vec3 down;
    Transform();
    Transform(const Vec3& position, const Vec3& rotation, const Vec3& scale);
    void UpdateVectors();
};