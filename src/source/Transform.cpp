#include <engine/Transform.h>

Transform::Transform()
{
    this->position.x = 0.0f;
    this->position.y = 0.0f;
    this->position.z = 0.0f;

    this->rotation.x = 0.0f;
    this->rotation.y = 0.0f;
    this->rotation.z = 0.0f;

    this->scale.x = 1.0f;
    this->scale.y = 1.0f;
    this->scale.z = 1.0f;
}

void Transform::UpdateVectors() {
    Mat4 mat;
    mat.Identify();
    Mat4::Rotation(mat, this->rotation, &mat);
    
    this->forward = Vec3(0, 0, 1) * mat;
    this->back = Vec3(0, 0, -1) * mat;
    this->left = Vec3(-1, 0, 0) * mat;
    this->right = Vec3(1, 0, 0) * mat;
    this->up = Vec3::CrossProduct(this->forward, this->right);
    this->down = Vec3::CrossProduct(this->back, this->left);
}