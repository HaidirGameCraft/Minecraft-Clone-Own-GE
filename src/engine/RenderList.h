#pragma once

#include <memory>
#include <vector>

#include <engine/GameObject.h>
#include <engine/Light.h>

class GLRenderList {
public:
    static std::vector<std::shared_ptr<GameObject>> gameobject_list;
    static std::vector<std::shared_ptr<Light>> light_list;
    static void clear();
};