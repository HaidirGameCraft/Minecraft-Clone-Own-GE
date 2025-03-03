#include <engine/Camera.h>
#include <cmath>

PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float near, float far)
{
    this->fov = fov;
    this->aspect = aspect;
    this->near = near;
    this->far = far;
}

void PerspectiveCamera::Create_Projection()
{
    const float DEG2RAD = std::acos(-1) / 180;
    float f = std::tan((fov / 2) * DEG2RAD);
    float range = near - far;
    
    transform.projection_matrix.Identify();
    transform.projection_matrix[0] = 1 / (f * aspect);
    transform.projection_matrix[5] = 1 / f;
    transform.projection_matrix[10] = (near + far) / range;
    transform.projection_matrix[11] = -1;
    transform.projection_matrix[14] = (2 * near * far) / range;
    transform.projection_matrix[15] = 0;
}

void PerspectiveCamera::UpdateMatrix()
{
    Mat4 matrix;
    matrix.Identify();
    transform.world_matrix.Identify();

    Mat4::Rotation(matrix, transform.rotation, &matrix);
    Mat4::Translate(matrix, transform.position, &matrix);

    Mat4::Inverse(matrix, transform.world_matrix);

    this->transform.UpdateVectors();
}