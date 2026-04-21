#pragma once

#include "SceneTypes.h"

namespace Mantra {

class SceneLoader
{
public:
    static TextureAsset LoadImage(const std::string& path, TextureSemantic semantic = TextureSemantic::BaseColor,
                                  bool sRGB = true);

    static Mesh LoadMesh(const std::string& path);
};

}  // namespace Mantra
