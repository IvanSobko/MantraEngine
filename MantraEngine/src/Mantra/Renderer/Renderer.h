#pragma once

#include "RenderCommand.h"
#include "RendererAPI.h"

namespace Mantra {

class Renderer
{
public:
    static void BeginScene();
    static void EndScene();

    static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

    inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
};

}  // namespace Mantra