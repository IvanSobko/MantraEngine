#include "VertexArray.h"

#include "Renderer.h"

#include "OpenGL/OpenGLVertexArray.h"

namespace Mantra {
VertexArray* VertexArray::Create() {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            ME_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL: return new OpenGLVertexArray();
    }

    ME_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}
}  // namespace Mantra