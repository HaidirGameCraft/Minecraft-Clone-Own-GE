#include <engine/Texture.h>
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture()
{

}

Texture::Texture(const char* filepath) {
    this->path = filepath;
}

Texture::~Texture() {
    glDeleteTextures(1, &this->texture_id);
}

void Texture::GenerateTexture() {
    
    if( change == false )  return;
    uint8_t* imageData = stbi_load(this->path.c_str(), &this->width, &this->height, &this->component, STBI_rgb_alpha);


    if( !glIsTexture(this->texture_id) )
        glCreateTextures(GL_TEXTURE_2D, 1, &this->texture_id);

    glBindTexture(GL_TEXTURE_2D, this->texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if( imageData != nullptr ) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        printf("Error> Failed to load Image\n");
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(imageData);

    this->change = false;
}

uint32_t& Texture::GetID() {
    return this->texture_id;
}