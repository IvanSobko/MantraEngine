#include "OpenGLTexture.h"
#include "ME_PCH.h"

#include "Mantra/Renderer/Renderer.h"

#include <glad/glad.h>
#include "stb_image.h"

namespace Mantra {

OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : mPath(path) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    if (!data) {
        ME_CORE_FATAL("Failed to load image '{0}': {1}", path, stbi_failure_reason());
    }

    mWidth = width;
    mHeight = height;
    mChannels = channels;
    ME_CORE_INFO("Loaded image '{0}' (width={1}, height={2}, channels={3})", path, mWidth, mHeight, mChannels);

    CreateGLTexture(width, height, channels, data);
    stbi_image_free(data);
}

OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, uint32_t channels, const uint8_t* pixelData)
    : mPath(""), mWidth(width), mHeight(height), mChannels(channels) {

    ME_CORE_INFO("Creating texture from pixel data (width={0}, height={1}, channels={2})", width, height, channels);
    CreateGLTexture(width, height, channels, pixelData);
}

void OpenGLTexture2D::CreateGLTexture(uint32_t width, uint32_t height, uint32_t channels, const uint8_t* data) {
    if (!data) {
        ME_CORE_ERROR("Cannot create texture with null pixel data");
        mRendererID = 0;
        return;
    }

    // Determine GL format based on channel count
    GLenum internalFormat = 0, dataFormat = 0;
    if (channels == 4) {
        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;
    } else if (channels == 3) {
        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;
    } else if (channels == 1) {
        internalFormat = GL_R8;
        dataFormat = GL_RED;
    } else {
        ME_CORE_ERROR("Unsupported channel count: {0}", channels);
        mRendererID = 0;
        return;
    }

    // Create and configure GL texture
    glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
    glTextureStorage2D(mRendererID, 1, internalFormat, width, height);
    glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload pixel data
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTextureSubImage2D(mRendererID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
}

OpenGLTexture2D::~OpenGLTexture2D() {
    glDeleteTextures(1, &mRendererID);
}

void OpenGLTexture2D::Bind(uint32_t slot) const {
    glBindTextureUnit(slot, mRendererID);
}

}  // namespace Mantra