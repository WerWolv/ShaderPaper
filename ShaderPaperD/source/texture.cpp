#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace wlp::gl {

    Texture::Texture(const std::string &path) {
        // Load image and convert it to RGBA
        this->m_imageData = stbi_load(path.c_str(), &this->m_width, &this->m_height, &this->m_channels, STBI_rgb);

        if (this->m_imageData == nullptr) {
            printf("[*] Error loading texture!\n");
            return;
        }

        // Generate and bind texture
        glGenTextures(1, &this->m_textureId);
        glBindTexture(GL_TEXTURE_2D, this->m_textureId);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Add image data to texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->m_width, this->m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->m_imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    Texture::~Texture() {
        if (this->m_imageData != nullptr)
            stbi_image_free(this->m_imageData);
    }

    void Texture::bind(GLint slot) {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, this->m_textureId);
    }

    const std::int32_t& Texture::getWidth() {
        return this->m_width;
    }

    const std::int32_t& Texture::getHeight() {
        return this->m_height;
    }

}