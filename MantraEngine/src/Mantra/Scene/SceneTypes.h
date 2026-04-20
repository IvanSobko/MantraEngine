#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <glm/glm.hpp>

namespace Mantra {

using SceneID = uint32_t;

constexpr SceneID kInvalidSceneID = static_cast<SceneID>(-1);

struct SceneTransform {
    glm::vec3 translation = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
};

struct SceneVertex {
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 normal = glm::vec3(0.0f);
    glm::vec2 uv = glm::vec2(0.0f);
};

struct Mesh {
    std::string name;
    std::vector<SceneVertex> vertices;
    std::vector<uint32_t> indices;
    SceneID materialID = kInvalidSceneID;
};

struct MeshInstance {
    SceneID meshID = kInvalidSceneID;
    SceneTransform transform;
};

enum class TextureSemantic : uint8_t { BaseColor = 0, Normal, MetallicRoughness, Emissive, Occlusion };

struct TextureAsset {
    std::string name;
    std::string path;
    TextureSemantic semantic = TextureSemantic::BaseColor;
    bool sRGB = true;

    // CPU-side pixel data
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t channels = 0;
    std::vector<uint8_t> pixelData;
};

struct Material {
    std::string name;

    glm::vec4 baseColor = glm::vec4(1.0f);
    glm::vec3 emissive = glm::vec3(0.0f);
    float metallic = 1.0f;
    float roughness = 1.0f;
    float normalScale = 1.0f;
    float occlusionStrength = 1.0f;

    SceneID baseColorTextureID = kInvalidSceneID;
    SceneID normalTextureID = kInvalidSceneID;
    SceneID metallicRoughnessTextureID = kInvalidSceneID;
    SceneID emissiveTextureID = kInvalidSceneID;
    SceneID occlusionTextureID = kInvalidSceneID;
};

}  // namespace Mantra