#include "TextureLoader.h"

#include <filesystem>
#include "stb_image.h"

namespace Mantra {

TextureAsset TextureLoader::LoadImage(const std::string& path, TextureSemantic semantic, bool sRGB) {
    TextureAsset asset;
    asset.path = path;
    asset.semantic = semantic;
    asset.sRGB = sRGB;

    std::filesystem::path p(path);
    asset.name = p.stem().string();

    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    if (!data) {
        ME_CORE_FATAL("Failed to load image '{0}': {1}", path, stbi_failure_reason());
        return asset;  // Return empty asset on failure
    }

    asset.width = static_cast<uint32_t>(width);
    asset.height = static_cast<uint32_t>(height);
    asset.channels = channels;

    size_t dataSize = static_cast<size_t>(width) * height * channels;
    asset.pixelData.resize(dataSize);
    std::memcpy(asset.pixelData.data(), data, dataSize);

    stbi_image_free(data);
    return asset;
}

}  // namespace Mantra
