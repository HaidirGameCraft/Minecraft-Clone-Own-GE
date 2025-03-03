#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <engine/window.h>
#include <engine/Shader.h>

int main(int argc, char const *argv[])
{
    Window window("OpenGL", 800, 600);
    int result = window.Initialize();

    if( result != 0 )
    {
        std::cout << "Error" << std::endl;
        return -1;
    }

    window.renderer.SetColor(1.0, 1.0, 0.0, 1.0);
    window.Render();
    window.Clear();

    return 0;
}
