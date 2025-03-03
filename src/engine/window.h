#pragma once

#include <GLFW/glfw3.h>
#include <engine/OpenGL.h>

class Window {
public:
    GLFWwindow* window;
    int width;
    int height;
    const char* text;
    OpenGLRenderer renderer;
    void* keyboard_handle = nullptr;

    Window(const char* text, int width, int height);
    void SetSize(int width, int height);
    int Initialize();
    void Render();
    void Clear();
private:
};