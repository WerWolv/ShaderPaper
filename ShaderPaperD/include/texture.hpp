#pragma once

#include <glad/gl.h>
#include <cstdint>
#include <string>

namespace wlp::gl {

    /**
     * A texture loaded from png, jpg, gif, etc.
     */
    class Texture {
    public:
        /**
         * Constructor
         * @param path Path to texture to load
         */
        Texture(const std::string &path);
        ~Texture();

        /**
         * Bind texture to slot
         * @param slot Slot to bind texture to
         */
        void bind(GLint slot);

        /**
         * Get texture width
         * @return width
         */
        const std::int32_t& getWidth();
        /**
         * Get texture height
         * @return height
         */
        const std::int32_t& getHeight();

    private:
        std::uint8_t *m_imageData = nullptr;
        std::int32_t m_width, m_height, m_channels;
        GLuint m_textureId;
    };

}