#pragma once

#include <engine/Mesh.h>
#include <engine/Material.h>
#include <engine/Transform.h>

class GameObject
{
public:
    Mesh* mesh;
    Material* material;
    Transform transform;
    unsigned int id_shader;
    GameObject();
    GameObject(Mesh* mesh);

    static GameObject* CreateGameObject();
    static GameObject* CreateGameObject(Mesh* mesh);

    void UpdateMatrix();
};