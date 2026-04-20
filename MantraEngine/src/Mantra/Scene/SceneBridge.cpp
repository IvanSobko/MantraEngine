#include "SceneBridge.h"

#include "Mantra/Renderer/Buffer.h"
#include "Mantra/Renderer/VertexArray.h"
#include "Mantra/Renderer/Texture.h"

namespace Mantra {

std::shared_ptr<VertexArray> CreateVAFromMesh(const Mesh& mesh) {
    std::vector<float> vertexData;
    vertexData.reserve(mesh.vertices.size() * 5);

    for (const SceneVertex& vertex : mesh.vertices) {
        vertexData.push_back(vertex.position.x);
        vertexData.push_back(vertex.position.y);
        vertexData.push_back(vertex.position.z);
        vertexData.push_back(vertex.uv.x);
        vertexData.push_back(vertex.uv.y);
    }

    auto vertexArray = VertexArray::Create();

    auto vertexBuffer =
        VertexBuffer::Create(vertexData.data(), static_cast<uint32_t>(vertexData.size() * sizeof(float)));
    vertexBuffer->SetLayout({{ShaderDataType::Float3, "a_Position"}, {ShaderDataType::Float2, "a_TexCoord"}});
    vertexArray->AddVertexBuffer(vertexBuffer);

    std::vector<uint32_t> indices = mesh.indices;
    auto indexBuffer = IndexBuffer::Create(indices.data(), static_cast<uint32_t>(indices.size()));
    vertexArray->SetIndexBuffer(indexBuffer);

    return vertexArray;
}

std::shared_ptr<Texture2D> CreateTextureFromAsset(const TextureAsset& texture) {
    if (texture.pixelData.empty()) {
        return nullptr;
    }

    return Texture2D::Create(texture.width, texture.height, texture.channels, texture.pixelData.data());
}

}  // namespace Mantra