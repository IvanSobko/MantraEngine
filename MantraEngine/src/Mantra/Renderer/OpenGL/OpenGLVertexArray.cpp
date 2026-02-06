#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Mantra {

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
    switch (type) {
        case Mantra::ShaderDataType::Float: return GL_FLOAT;
        case Mantra::ShaderDataType::Float2: return GL_FLOAT;
        case Mantra::ShaderDataType::Float3: return GL_FLOAT;
        case Mantra::ShaderDataType::Float4: return GL_FLOAT;
        case Mantra::ShaderDataType::Mat3: return GL_FLOAT;
        case Mantra::ShaderDataType::Mat4: return GL_FLOAT;
        case Mantra::ShaderDataType::Int: return GL_INT;
        case Mantra::ShaderDataType::Int2: return GL_INT;
        case Mantra::ShaderDataType::Int3: return GL_INT;
        case Mantra::ShaderDataType::Int4: return GL_INT;
        case Mantra::ShaderDataType::Bool: return GL_BOOL;
    }

    ME_CORE_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
}

OpenGLVertexArray::OpenGLVertexArray() {
    glCreateVertexArrays(1, &mRendererID);
}

OpenGLVertexArray::~OpenGLVertexArray() {
    glDeleteVertexArrays(1, &mRendererID);
}

void OpenGLVertexArray::Bind() const {
    glBindVertexArray(mRendererID);
}

void OpenGLVertexArray::Unbind() const {
    glBindVertexArray(0);
}

void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) {
    ME_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

    glBindVertexArray(mRendererID);
    vertexBuffer->Bind();

    uint32_t index = 0;
    const auto& layout = vertexBuffer->GetLayout();
    for (const auto& element : layout) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type),
                              element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(),
                              (const void*)(intptr_t)element.Offset);
        index++;
    }

    mVertexBuffers.push_back(vertexBuffer);
}

void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) {
    glBindVertexArray(mRendererID);
    indexBuffer->Bind();

    mIndexBuffer = indexBuffer;
}

}  // namespace Mantra