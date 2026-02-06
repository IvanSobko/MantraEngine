#include "Shader.h"

#include "OpenGL/OpenGLShader.h"
#include "Renderer.h"

namespace Mantra {

Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc) {
    switch (Renderer::GetAPI())

    {
        case RendererAPI::API::None:
            ME_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL: return new OpenGLShader(vertexSrc, fragmentSrc);
    }

    ME_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

}  // namespace Mantra