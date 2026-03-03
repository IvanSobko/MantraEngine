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

    GLenum internalFormat = 0, dataFormat = 0;
    if (mChannels == 4) {
        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;
    } else if (mChannels == 3) {
        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;
    }

    glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);

    int levels = 1;
    glTextureStorage2D(mRendererID, levels, internalFormat, mWidth, mHeight);

    glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTextureSubImage2D(mRendererID, 0, 0, 0, mWidth, mHeight, dataFormat, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
}

OpenGLTexture2D::~OpenGLTexture2D() {
    glDeleteTextures(1, &mRendererID);
}

void OpenGLTexture2D::Bind(uint32_t slot) const {
    glBindTextureUnit(slot, mRendererID);
}

}  // namespace Mantra