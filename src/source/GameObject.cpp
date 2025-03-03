#include <engine/GameObject.h>

#include <memory>
#include <engine/RenderList.h>

GameObject::GameObject()
{
    this->mesh = nullptr;
    this->id_shader = 0;
}

GameObject::GameObject(Mesh* mesh)
{
    this->mesh = mesh;
}

void GameObject::UpdateMatrix()
{
    this->transform.local_matrix.Identify();
    Mat4::Scale(this->transform.local_matrix, this->transform.scale, &this->transform.local_matrix);
    Mat4::Translate(this->transform.local_matrix, this->transform.position, &this->transform.local_matrix);
    Mat4::Rotation(this->transform.local_matrix, this->transform.rotation, &this->transform.local_matrix);

}

GameObject* GameObject::CreateGameObject() {
    std::shared_ptr<GameObject> game_object = std::make_shared<GameObject>();
    GLRenderList::gameobject_list.push_back( game_object );
    return game_object.get();
}

GameObject* GameObject::CreateGameObject(Mesh* mesh) {
    std::shared_ptr<GameObject> game_object = std::make_shared<GameObject>(mesh);
    GLRenderList::gameobject_list.push_back( game_object );
    return game_object.get();
}