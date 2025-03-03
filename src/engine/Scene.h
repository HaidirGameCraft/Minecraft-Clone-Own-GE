#pragma once

#include <engine/GameObject.h>
#include <engine/Light.h>
#include <vector>
#include <map>
#include <string>

class Scene {
public:
    std::vector<GameObject> game_objects;
    std::vector<Light> lights;
    std::map<std::string, Mesh*> meshes;
    std::map<std::string, Material*> material;


    void Clear();
};