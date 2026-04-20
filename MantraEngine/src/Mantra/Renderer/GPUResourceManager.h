#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace Mantra {

class Shader;
class VertexArray;
class Texture2D;
struct Mesh;
struct TextureAsset;
using SceneID = uint32_t;

/**
 * Centralized manager for all GPU resources (shaders, meshes, textures).
 * Owns and caches GPU resources, and handles creation from scene data.
 * 
 * Mirrors the CPU scene structure on the GPU:
 * - Shaders are named resources (by string)
 * - VertexArrays are indexed by SceneID (meshID)
 * - Textures are indexed by SceneID (textureID)
 */
class GPUResourceManager
{
public:
    GPUResourceManager() = default;
    ~GPUResourceManager() = default;

    std::shared_ptr<Shader> LoadShader(const std::string& name, const std::string& filepath);
    void AddShader(const std::string& name, const std::shared_ptr<Shader>& shader);
    std::shared_ptr<Shader> GetShader(const std::string& name) const;

    std::shared_ptr<VertexArray> CreateVertexArrayFromMesh(SceneID meshID, const Mesh& mesh);
    void AddVertexArray(SceneID meshID, const std::shared_ptr<VertexArray>& va);
    std::shared_ptr<VertexArray> GetVertexArray(SceneID meshID) const;

    std::shared_ptr<Texture2D> CreateTextureFromAsset(SceneID textureID, const TextureAsset& asset);
    void AddTexture(SceneID textureID, const std::shared_ptr<Texture2D>& texture);
    std::shared_ptr<Texture2D> GetTexture(SceneID textureID) const;

    void Clear();

private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> mShaders;
    std::unordered_map<SceneID, std::shared_ptr<VertexArray>> mVertexArrays;
    std::unordered_map<SceneID, std::shared_ptr<Texture2D>> mTextures;
};

}  // namespace Mantra
