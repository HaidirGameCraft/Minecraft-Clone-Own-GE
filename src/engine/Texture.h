#pragma once

#include <stdint.h>
#include <string>

class Texture {
private:
    uint32_t texture_id;
    std::string path;
public:
    int width;
    int height;
    int component;
    bool change = true;
    Texture();
    Texture(const char* path);
    ~Texture();
    void GenerateTexture();
    uint32_t& GetID();
};