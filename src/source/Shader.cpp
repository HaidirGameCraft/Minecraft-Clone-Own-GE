#include <glad/glad.h>
#include <engine/Shader.h>

#include <string.h>
#include <fstream>
#include <iostream>

std::map<std::string, uint32_t> ShaderProgram::program_lists;

uint32_t ShaderProgram::CreateProgram(const char* name, const char* vertex_code, const char* fragment_code)
{
    unsigned int vertex_buffer = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragment_buffer = glCreateShader(GL_FRAGMENT_SHADER);


    int status = 0;
    char log[512];

    glShaderSource(vertex_buffer, 1, &vertex_code, NULL);
    glCompileShader(vertex_buffer);
    glGetShaderiv(vertex_buffer, GL_COMPILE_STATUS, &status);
    if( !status )
    {
        glGetShaderInfoLog(vertex_buffer, 512, NULL, log);
        std::cerr << "Err VERTEX SHADER: " << log << std::endl;
        return 0;
    }

    glShaderSource(fragment_buffer, 1, &fragment_code, NULL);
    glCompileShader(fragment_buffer);
    glGetShaderiv(fragment_buffer, GL_COMPILE_STATUS, &status);
    if( !status )
    {
        glGetShaderInfoLog(fragment_buffer, 512, NULL, log);
        std::cerr << "Err FRAGMENT SHADER: " << log << std::endl;
        return 0;
    }

    uint32_t program = glCreateProgram();
    glAttachShader(program, vertex_buffer);
    glAttachShader(program, fragment_buffer);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if( !status )
    {
        glGetProgramInfoLog(program, 512, NULL, log);
        std::cerr << "Err Program SHADER: " << log << std::endl;
        return 0;
    }

    glDeleteShader(vertex_buffer);
    glDeleteShader(fragment_buffer);

    program_lists[name] = program;
    return program;
}

void ShaderProgram::CreatePrograms(std::vector<ShaderFile>& shaderfiles) {
    for(int i = 0; i < shaderfiles.size(); i++)
    {
        CreateProgram(shaderfiles[i].name.c_str(), ReadFile(shaderfiles[i].vertex_path.c_str()), ReadFile(shaderfiles[i].fragment_path.c_str()));
    }
}

char* ShaderProgram::ReadFile(const char* path) {

    std::ifstream file(path, std::ios_base::binary);

    if( !file.is_open() )
    {
        std::cout << "Err> Cannot read" << std::endl;
        return NULL;
    }

    file.seekg(0, std::ios_base::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios_base::beg);

    char *buffer = (char*) malloc( size + 1 );
    memset(buffer, 0, size + 1);
    file.read(buffer, size);
    file.close();
    return buffer;
}

#define FIND_LOCATION(i, x) glGetUniformLocation(i, x)

void ShaderProgram::SetMatrix4(int program, std::string location, Mat4& mat) {
    int local = FIND_LOCATION(program, location.c_str());
    glUniformMatrix4fv(local, 1, false, mat.m);
}
void ShaderProgram::SetVector3(int program, std::string location, Vec3& vec) {
    int local = FIND_LOCATION(program, location.c_str());
    glUniform3f(local, vec.x, vec.y, vec.z);
}
void ShaderProgram::SetColor(int program, std::string location, Color& color) {
    int local = FIND_LOCATION(program, location.c_str());
    glUniform4f(local, color.r, color.g, color.b, color.a);
}
void ShaderProgram::SetBool(int program, std::string location, unsigned int value) {
    int local = FIND_LOCATION(program, location.c_str());
    glUniform1i(local, value);
}

void ShaderProgram::SetInt(int program, std::string location, int value) {
    int local = FIND_LOCATION(program, location.c_str());
    glUniform1i(local, value);
}