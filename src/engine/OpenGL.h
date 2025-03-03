#pragma once

#include <stdint.h>
#include <engine/Vector.h>
#include <vector>
#include <memory>
#include <engine/GameObject.h>
#include <engine/Camera.h>

class RendererObjectList
{
public:
    static std::vector<std::shared_ptr<GameObject>> list;
};

class OpenGLRenderer
{
public:

    int width;
    int height;

    void (*RenderUpdate)();
    // Color background
    Color color;
    float alpha;
    OpenGLRenderer(int width, int height);
    void SetColor(float r, float g, float b, float a);
    void Render(PerspectiveCamera* camera);
    void RenderObject(GameObject* object, Camera* camera);
};