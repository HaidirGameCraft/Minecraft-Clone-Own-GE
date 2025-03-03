#include <engine/RenderList.h>

std::vector<std::shared_ptr<GameObject>> GLRenderList::gameobject_list;
std::vector<std::shared_ptr<Light>> GLRenderList::light_list;

void GLRenderList::clear() {
    GLRenderList::gameobject_list.clear();
    GLRenderList::light_list.clear();
}