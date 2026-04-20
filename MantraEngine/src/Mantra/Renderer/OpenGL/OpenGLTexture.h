#pragma once

#include "Mantra/Renderer/Texture.h"

namespace Mantra {

class OpenGLTexture2D : public Texture2D
{
public:
    OpenGLTexture2D(const std::string& path);
    OpenGLTexture2D(uint32_t width, uint32_t height, uint32_t channels, const uint8_t* pixelData);
    virtual ~OpenGLTexture2D();

    virtual uint32_t GetWidth() const override { return mWidth; }
    virtual uint32_t GetHeight() const override { return mHeight; }

    virtual void Bind(uint32_t unit = 0) const override;

private:
    void CreateGLTexture(uint32_t width, uint32_t height, uint32_t channels, const uint8_t* data);

    std::string mPath;
    uint32_t mWidth, mHeight;
    uint8_t mChannels;
    uint32_t mRendererID;
};

}  // namespace Mantra