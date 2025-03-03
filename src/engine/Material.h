#pragma once

#include <engine/Vector.h>
#include <engine/Texture.h>

#include <vector>
#include <memory>

class Material {
public:
    Color color;
    Texture* texture;
    unsigned int id_program;
};