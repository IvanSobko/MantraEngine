#pragma once

#include "SceneTypes.h"

namespace Mantra {

class Scene
{
public:
    SceneID AddMesh(const Mesh& mesh) {
        meshes.push_back(mesh);
        return static_cast<SceneID>(meshes.size() - 1);
    }

    SceneID AddMaterial(const Material& material) {
        materials.push_back(material);
        return static_cast<SceneID>(materials.size() - 1);
    }

    SceneID AddTexture(const TextureAsset& texture) {
        textures.push_back(texture);
        return static_cast<SceneID>(textures.size() - 1);
    }

    SceneID AddInstance(const MeshInstance& instance) {
        instances.push_back(instance);
        return static_cast<SceneID>(instances.size() - 1);
    }

    void Clear() {
        meshes.clear();
        materials.clear();
        textures.clear();
        instances.clear();
    }

public:
    std::vector<Mesh> meshes;
    std::vector<Material> materials;
    std::vector<TextureAsset> textures;
    std::vector<MeshInstance> instances;
};

}  // namespace Mantra