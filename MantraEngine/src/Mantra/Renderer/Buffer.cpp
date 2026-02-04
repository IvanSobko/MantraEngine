#include "Buffer.h"
#include "ME_PCH.h"

#include "Renderer.h"

#include "OpenGL/OpenGLBuffer.h"

namespace Mantra {

VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::None: ME_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::OpenGL: return new OpenGLVertexBuffer(vertices, size);
    }

    ME_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::None: ME_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::OpenGL: return new OpenGLIndexBuffer(indices, size);
    }

    ME_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

uint32_t BufferElement::GetComponentCount() const {
    switch (Type) {
        case ShaderDataType::Float: return 1;
        case ShaderDataType::Float2: return 2;
        case ShaderDataType::Float3: return 3;
        case ShaderDataType::Float4: return 4;
        case ShaderDataType::Mat3: return 3 * 3;
        case ShaderDataType::Mat4: return 4 * 4;
        case ShaderDataType::Int: return 1;
        case ShaderDataType::Int2: return 2;
        case ShaderDataType::Int3: return 3;
        case ShaderDataType::Int4: return 4;
        case ShaderDataType::Bool: return 1;
    }

    ME_CORE_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
}

BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements) : mElements(elements) {
    CalculateOffsetsAndStride();
}

void BufferLayout::CalculateOffsetsAndStride() {
    uint32_t offset = 0;
    mStride = 0;
    for (BufferElement& element : mElements) {
        element.Offset = offset;
        offset += element.Size;
    }
    mStride = offset;
}

}  // namespace Mantra