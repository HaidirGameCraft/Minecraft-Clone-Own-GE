#include <engine/Raycast.h>

Raycast::Raycast(Vec3 origin, Vec3 dir) {
    this->origin = origin;
    this->direction = dir;
}
    
std::vector<RayIntersect<BoxCollider>> Raycast::IntersectBoxes(std::vector<BoxCollider>& objects, float max_distance) {
    std::vector<RayIntersect<BoxCollider>> colliders;

    for(int i = 0; i < objects.size(); i++)
    {
        RayIntersect<BoxCollider> collide = this->IntersectBox(objects[i], max_distance);
        if( collide.object != nullptr )
        {
            colliders.push_back( collide );
        }
    }

    return colliders;
}

RayIntersect<BoxCollider> Raycast::IntersectBox(BoxCollider& collider, float max_distance) {
    RayIntersect<BoxCollider> box_collider;

    float DXmin = (collider.get_min().x - this->origin.x) / this->direction.x;
    float DXmax = (collider.get_max().x - this->origin.x) / this->direction.x;

    if( DXmax < DXmin )
    {
        float tmp = DXmax;
        DXmax = DXmin;
        DXmin = tmp;
    }

    float DYmin = (collider.get_min().y - this->origin.y) / this->direction.y;
    float DYmax = (collider.get_max().y - this->origin.y) / this->direction.y;

    if( DYmax < DYmin )
    {
        float tmp = DYmax;
        DYmax = DYmin;
        DYmin = tmp;
    }

    float tmin = DXmin < DYmin ? DYmin : DXmin;
    float tmax = DXmax < DYmax ? DXmax : DYmax;

    if( tmin > tmax )
        return {Vec3(), 0, nullptr};
    

    float DZmin = (collider.get_min().z - this->origin.z) / this->direction.z;
    float DZmax = (collider.get_max().z - this->origin.z) / this->direction.z;

    if( DZmax < DZmin )
    {
        float tmp = DZmax;
        DZmax = DZmin;
        DZmin = tmp;
    }

    tmin = DZmin < tmin ? tmin : DZmin;
    tmax = DZmax < tmax ? DZmax : tmax;

    if( tmin > tmax )
        return {Vec3(), 0, nullptr};

    float t = tmin < tmax ? tmin : tmax;

    if( t <= 0 || t > max_distance )
        return {Vec3(), 0, nullptr};

    box_collider.point = this->direction * t + this->origin;
    box_collider.object = &collider;
    box_collider.t = t;

    return box_collider;
}

bool Raycast::CalculateBoxIntersect(BoxCollider& collider, float max_distance) {
    float DXmin = (collider.get_min().x - this->origin.x) / this->direction.x;
    float DXmax = (collider.get_max().x - this->origin.x) / this->direction.x;

    if( DXmax < DXmin )
    {
        float tmp = DXmax;
        DXmax = DXmin;
        DXmin = tmp;
    }

    float DYmin = (collider.get_min().y - this->origin.y) / this->direction.y;
    float DYmax = (collider.get_max().y - this->origin.y) / this->direction.y;

    if( DYmax < DYmin )
    {
        float tmp = DYmax;
        DYmax = DYmin;
        DYmin = tmp;
    }

    float tmin = DXmin < DYmin ? DYmin : DXmin;
    float tmax = DXmax < DYmax ? DXmax : DYmax;

    if( tmin > tmax )
        return false;
    

    float DZmin = collider.get_min().z - this->origin.z;
    float DZmax = collider.get_max().z - this->origin.z;

    if( DZmax < DZmin )
    {
        float tmp = DZmax;
        DZmax = DZmin;
        DZmin = tmp;
    }

    tmin = DZmin < tmin ? tmin : DZmin;
    tmax = DZmax < tmax ? DZmax : tmax;

    if( tmin > tmax )
        return false;

    return true;
}