#include "VertexArray.h"

#include "Renderer.h"

#include "OpenGL/OpenGLVertexArray.h"

namespace Mantra {

std::shared_ptr<VertexArray> VertexArray::Create() {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            ME_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL: return std::make_shared<OpenGLVertexArray>();
    }

    ME_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

}  // namespace Mantra