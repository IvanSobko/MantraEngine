#pragma once

#include <memory>

#include "SceneTypes.h"

namespace Mantra {

class VertexArray;
class Texture2D;

/**
 * Create a VertexArray from a Mesh, uploading vertex/index data to GPU buffers.
 * @param mesh: Mesh with CPU-side vertex/index data
 * @return Shared pointer to GPU VertexArray with data uploaded and ready for rendering
 */

std::shared_ptr<VertexArray> CreateVAFromMesh(const Mesh& mesh);

/**
 * Upload a TextureAsset to GPU, creating a GPU Texture2D resource.
 * @param texture: TextureAsset with CPU pixel data loaded
 * @return Shared pointer to GPU Texture2D, or nullptr on failure
 */
std::shared_ptr<Texture2D> CreateTextureFromAsset(const TextureAsset& texture);

}  // namespace Mantra