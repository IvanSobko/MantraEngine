#include "GPUResourceManager.h"
#include "Mantra/Scene/SceneTypes.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"

namespace Mantra {

GPUResourceManager::GPUResourceManager() {
    // Create a 1x1 white texture as default for textureless rendering
    // Store at kInvalidSceneID so meshes with no texture default to white
    uint8_t whitePixel[4] = {255, 255, 255, 255};  // RGBA white
    auto defaultWhiteTexture = Texture2D::Create(1, 1, 4, whitePixel);
    if (!defaultWhiteTexture) {
        ME_CORE_ERROR("Failed to create default white texture");
    } else {
        mTextures[kInvalidSceneID] = defaultWhiteTexture;
    }
}

GPUResourceManager::~GPUResourceManager() {
    Clear();
}

std::shared_ptr<Shader> GPUResourceManager::LoadShader(const std::string& name, const std::string& filepath) {
    auto shader = Shader::Create(filepath);
    if (!shader) {
        ME_CORE_ERROR("Failed to load shader '{0}' from '{1}'", name, filepath);
        return nullptr;
    }

    AddShader(name, shader);
    return shader;
}

void GPUResourceManager::AddShader(const std::string& name, const std::shared_ptr<Shader>& shader) {
    if (!shader) {
        ME_CORE_ERROR("Cannot add null shader '{0}'", name);
        return;
    }
    if (mShaders.find(name) != mShaders.end()) {
        ME_CORE_WARN("Shader '{0}' already exists, replacing", name);
    }
    mShaders[name] = shader;
}

std::shared_ptr<Shader> GPUResourceManager::GetShader(const std::string& name) const {
    auto it = mShaders.find(name);
    if (it == mShaders.end()) {
        ME_CORE_WARN("Shader '{0}' not found", name);
        return nullptr;
    }
    return it->second;
}

std::shared_ptr<VertexArray> GPUResourceManager::CreateVertexArrayFromMesh(SceneID meshID, const Mesh& mesh) {
    std::vector<float> vertexData;
    vertexData.reserve(mesh.vertices.size() * 8);  // pos(3) + normal(3) + uv(2)

    for (const SceneVertex& vertex : mesh.vertices) {
        // Position
        vertexData.push_back(vertex.position.x);
        vertexData.push_back(vertex.position.y);
        vertexData.push_back(vertex.position.z);
        // Normal
        vertexData.push_back(vertex.normal.x);
        vertexData.push_back(vertex.normal.y);
        vertexData.push_back(vertex.normal.z);
        // TexCoord
        vertexData.push_back(vertex.uv.x);
        vertexData.push_back(vertex.uv.y);
    }

    auto vertexArray = VertexArray::Create();

    auto vertexBuffer =
        VertexBuffer::Create(vertexData.data(), static_cast<uint32_t>(vertexData.size() * sizeof(float)));
    vertexBuffer->SetLayout({{ShaderDataType::Float3, "a_Position"},
                             {ShaderDataType::Float3, "a_Normal"},
                             {ShaderDataType::Float2, "a_TexCoord"}});
    vertexArray->AddVertexBuffer(vertexBuffer);

    std::vector<uint32_t> indices = mesh.indices;
    auto indexBuffer = IndexBuffer::Create(indices.data(), static_cast<uint32_t>(indices.size()));
    vertexArray->SetIndexBuffer(indexBuffer);

    AddVertexArray(meshID, vertexArray);
    return vertexArray;
}

void GPUResourceManager::AddVertexArray(SceneID meshID, const std::shared_ptr<VertexArray>& va) {
    if (!va) {
        ME_CORE_ERROR("Cannot add null VertexArray for mesh {0}", meshID);
        return;
    }
    if (mVertexArrays.find(meshID) != mVertexArrays.end()) {
        ME_CORE_WARN("VertexArray for mesh {0} already exists, replacing", meshID);
    }
    mVertexArrays[meshID] = va;
}

std::shared_ptr<VertexArray> GPUResourceManager::GetVertexArray(SceneID meshID) const {
    auto it = mVertexArrays.find(meshID);
    if (it == mVertexArrays.end()) {
        ME_CORE_WARN("VertexArray for mesh {0} not found", meshID);
        return nullptr;
    }
    return it->second;
}

std::shared_ptr<Texture2D> GPUResourceManager::CreateTextureFromAsset(SceneID textureID, const TextureAsset& asset) {
    if (asset.pixelData.empty()) {
        ME_CORE_ERROR("Cannot create texture {0} with no pixel data", textureID);
        return nullptr;
    }

    auto texture = Texture2D::Create(asset.width, asset.height, asset.channels, asset.pixelData.data());
    if (!texture) {
        ME_CORE_ERROR("Failed to create texture {0} from asset", textureID);
        return nullptr;
    }

    AddTexture(textureID, texture);
    return texture;
}

void GPUResourceManager::AddTexture(SceneID textureID, const std::shared_ptr<Texture2D>& texture) {
    if (!texture) {
        ME_CORE_ERROR("Cannot add null Texture2D for texture {0}", textureID);
        return;
    }
    if (mTextures.find(textureID) != mTextures.end()) {
        ME_CORE_WARN("Texture2D {0} already exists, replacing", textureID);
    }
    mTextures[textureID] = texture;
}

std::shared_ptr<Texture2D> GPUResourceManager::GetTexture(SceneID textureID) const {
    auto it = mTextures.find(textureID);
    if (it == mTextures.end()) {
        ME_CORE_WARN("Texture2D {0} not found", textureID);
        return nullptr;
    }
    return it->second;
}

void GPUResourceManager::Clear() {
    mShaders.clear();
    mVertexArrays.clear();
    mTextures.clear();
}

}  // namespace Mantra
