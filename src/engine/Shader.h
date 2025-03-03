#pragma once

#include <vector>
#include <string>
#include <map>

#include <engine/Mat4.h>
#include <engine/Texture.h>
#include <engine/Vector.h>

struct ShaderFile
{
    std::string name;
    std::string vertex_path;
    std::string fragment_path;
};

class ShaderProgram
{
public:
    static std::map<std::string, uint32_t> program_lists;
    static void CreatePrograms(std::vector<ShaderFile>& shaderfiles);
    static uint32_t CreateProgram(const char* name, const char* vertex_code, const char* fragment_code);
    static char* ReadFile(const char* path);

    // For Configuration
    static void SetMatrix4(int program, std::string location, Mat4& mat);
    static void SetVector3(int program, std::string location, Vec3& vec);
    static void SetColor(int program, std::string location, Color& color);
    static void SetBool(int program, std::string location, unsigned int value);
    static void SetInt(int program, std::string location, int value);
};