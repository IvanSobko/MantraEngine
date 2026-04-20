#include "Texture.h"
#include "ME_PCH.h"

#include "Mantra/Renderer/OpenGL/OpenGLTexture.h"
#include "Renderer.h"

namespace Mantra {

std::shared_ptr<Texture2D> Texture2D::Create(const std::string& path) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            ME_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL: return std::make_shared<OpenGLTexture2D>(path);
    }

    ME_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

std::shared_ptr<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, uint32_t channels,
                                             const uint8_t* pixelData) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            ME_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL: return std::make_shared<OpenGLTexture2D>(width, height, channels, pixelData);
    }

    ME_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

}  // namespace Mantra