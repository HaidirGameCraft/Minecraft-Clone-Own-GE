#pragma once
#include <engine/Vector.h>
#include <engine/Mesh.h>
#include <engine/GameObject.h>

class BoxCollider {
private:
    Vec3 min;
    Vec3 max;
public:
    BoxCollider();
    BoxCollider(const Vec3& min, const Vec3& max);
    void CalculateMesh(Mesh& mesh);
    void CalculateGameObject(GameObject& gameObject);
    Vec3& get_min();
    Vec3& get_max();
};