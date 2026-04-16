#include "ME_PCH.h"

#include "Framebuffer.h"
#include "Renderer.h"

#include "OpenGL/OpenGLFrameBuffer.h"

namespace Mantra {

std::shared_ptr<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            ME_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL: return std::make_shared<OpenGLFramebuffer>(spec);
    }

    ME_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

}  // namespace Mantra