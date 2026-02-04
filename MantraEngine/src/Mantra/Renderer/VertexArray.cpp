#include "VertexArray.h"

#include "Renderer.h"

#include "OpenGL/OpenGLVertexArray.h"

namespace Mantra {
VertexArray* VertexArray::Create() {
    switch (Renderer::GetAPI()) {
        case RendererAPI::None: ME_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::OpenGL: return new OpenGLVertexArray();
    }

    ME_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}
}  // namespace Mantra 