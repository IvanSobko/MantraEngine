#pragma once

#include <memory>

#include "SceneTypes.h"

namespace Mantra {

class VertexArray;

std::shared_ptr<VertexArray> CreateVAFromMesh(const Mesh& mesh);

}  // namespace Mantra