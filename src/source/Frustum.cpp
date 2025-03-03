#include <engine/Frustum.h>
#include <engine/Mat4.h>
#include <cmath>

Frustum::Frustum() {
    
};

Frustum::~Frustum()
{
    
}

void Frustum::CalculateFrustum(Camera& camera)
{
    
    PerspectiveCamera* perspective = static_cast<PerspectiveCamera*>(&camera);
    if( perspective != nullptr )
    {
        Mat4 proj = camera.transform.projection_matrix;
        Mat4 world_matrix = camera.transform.world_matrix;

        Mat4 projection = world_matrix * proj;

        planes[F_LEFT].normal = Vec3( projection[3] + projection[0], projection[7] + projection[4], projection[11] + projection[8]);
        planes[F_LEFT].distance = projection[15] + projection[12];

        planes[F_RIGHT].normal = Vec3( projection[3] - projection[0], projection[7] - projection[4], projection[11] - projection[8]);
        planes[F_RIGHT].distance = projection[15] - projection[12];

        planes[F_BOTTOM].normal = Vec3( projection[3] + projection[1], projection[7] + projection[5], projection[11] + projection[9]);
        planes[F_BOTTOM].distance = projection[15] + projection[13];

        planes[F_TOP].normal = Vec3( projection[3] - projection[1], projection[7] - projection[5], projection[11] - projection[9]);
        planes[F_TOP].distance = projection[15] - projection[13];

        planes[F_NEAR].normal = Vec3( projection[3] + projection[2], projection[7] + projection[6], projection[11] + projection[10]);
        planes[F_NEAR].distance = projection[15] + projection[14];

        planes[F_FAR].normal = Vec3( projection[3] - projection[2], projection[7] - projection[6], projection[11] - projection[10]);
        planes[F_FAR].distance = projection[15] - projection[14];
    }

    for(int i = 0; i < 6; i++)
    {
        float d = planes[i].distance;
        Vec3 n = planes[i].normal;
        float length = sqrt(n.x * n.x + n.y * n.y + n.z * n.z + d * d);
        if( length < 0.000001 )
            length = 0.000001;

        planes[i].normal = n / length;
        planes[i].distance = d / length;
    }
}

bool Frustum::IsPointInsideFrustum(Vec3 point) {
    for(int i = 0; i < 6; i++)
    {
        float d = planes[i].distance;
        Vec3 n = planes[i].normal;
     
        if( n.Dot( point ) + d <= 0 )
        {
            return false;
        }
    }

    return true;
}

bool Frustum::IsBoxInsideFrustum(BoxCollider* collider) {
    Vec3 &vmin = collider->get_min();
    Vec3 &vmax = collider->get_max();

    return (IsPointInsideFrustum(Vec3(vmin.x, vmin.y, vmin.z))) || IsPointInsideFrustum(Vec3(vmin.x, vmax.y, vmin.z)) ||
            IsPointInsideFrustum(Vec3(vmax.x, vmax.y, vmin.z)) || IsPointInsideFrustum(Vec3(vmax.x, vmin.y, vmin.z)) ||
            (IsPointInsideFrustum(Vec3(vmin.x, vmin.y, vmax.z))) || IsPointInsideFrustum(Vec3(vmin.x, vmax.y, vmax.z)) ||
            IsPointInsideFrustum(Vec3(vmax.x, vmax.y, vmax.z)) || IsPointInsideFrustum(Vec3(vmax.x, vmin.y, vmax.z));
}