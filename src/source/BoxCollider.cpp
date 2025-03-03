#include <engine/Collider.h>
#include <cmath>

BoxCollider::BoxCollider() {
    this->min = Vec3(INFINITY, INFINITY, INFINITY);
    this->max = Vec3(-INFINITY, -INFINITY, -INFINITY);
}
BoxCollider::BoxCollider(const Vec3& min, const Vec3& max) {
    this->min = min;
    this->max = max;
}

void BoxCollider::CalculateMesh(Mesh& mesh)
{
    for(int i = 0; i < mesh.vertices.size(); i += 3)
    {
        Vec3 vec(mesh.vertices[i * 3 + 0], mesh.vertices[i * 3 + 1], mesh.vertices[i * 3 + 2]);
        this->min.Min( vec );
        this->max.Max( vec );
    }
}

void BoxCollider::CalculateGameObject(GameObject& gameObject) {
    for(int i = 0; i < gameObject.mesh->vertices.size(); i += 3)
    {
        Vec3 vec(gameObject.mesh->vertices[i * 3 + 0], gameObject.mesh->vertices[i * 3 + 1], gameObject.mesh->vertices[i * 3 + 2]);
        vec = vec + gameObject.transform.position;
        this->min.Min( vec );
        this->max.Max( vec );
    }
}

Vec3& BoxCollider::get_min() {
    return this->min;
}
Vec3& BoxCollider::get_max() {
    return this->max;
}