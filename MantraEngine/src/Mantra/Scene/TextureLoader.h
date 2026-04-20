#pragma once

#include "SceneTypes.h"

namespace Mantra {

class TextureLoader
{
public:
    static TextureAsset LoadImage(const std::string& path, TextureSemantic semantic = TextureSemantic::BaseColor,
                                  bool sRGB = true);
};

}  // namespace Mantra
