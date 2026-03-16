#pragma once

#include "RendererAPI.h"

#include "glm/glm.hpp"

namespace Mantra {

class RenderCommand
{
public:
    static void Init();

    inline static void SetClearColor(const glm::vec4& color) { sRendererAPI->SetClearColor(color); }

    inline static void Clear() { sRendererAPI->Clear(); }

    inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) {
        sRendererAPI->DrawIndexed(vertexArray);
    }

    inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
        sRendererAPI->SetViewport(x, y, width, height);
    }

private:
    static RendererAPI* sRendererAPI;
};
}  // namespace Mantra