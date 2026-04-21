#include "SceneLoader.h"

#include <filesystem>
#include "stb_image.h"
#include "tiny_obj_loader.h"

namespace Mantra {

TextureAsset SceneLoader::LoadImage(const std::string& path, TextureSemantic semantic, bool sRGB) {
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

Mesh SceneLoader::LoadMesh(const std::string& path) {
    tinyobj::ObjReaderConfig reader_config;
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(path, reader_config)) {
        if (!reader.Error().empty()) {
            ME_CORE_ERROR("TinyObjReader: {0}", path, reader.Error());
        }
        ME_CORE_ERROR("Failed to load OBJ: {0}", path);
        return Mesh();
    }

    if (!reader.Warning().empty()) {
        ME_CORE_WARN("TinyObjReader: {0}", reader.Warning());
    }

    const tinyobj::attrib_t& attrib = reader.GetAttrib();
    const std::vector<tinyobj::shape_t>& shapes = reader.GetShapes();

    Mesh mesh;
    std::filesystem::path p(path);
    mesh.name = p.stem().string();

    // Iterate shapes and accumulate vertices/indices
    uint32_t vertexOffset = 0;
    for (const tinyobj::shape_t& shape : shapes) {
        const std::vector<tinyobj::index_t>& indices = shape.mesh.indices;

        for (const tinyobj::index_t& idx : indices) {
            SceneVertex vertex;

            if (static_cast<size_t>(idx.vertex_index) * 3 + 2 < attrib.vertices.size()) {
                vertex.position.x = attrib.vertices[3 * idx.vertex_index + 0];
                vertex.position.y = attrib.vertices[3 * idx.vertex_index + 1];
                vertex.position.z = attrib.vertices[3 * idx.vertex_index + 2];
            }

            if (idx.normal_index >= 0 && static_cast<size_t>(idx.normal_index) * 3 + 2 < attrib.normals.size()) {
                vertex.normal.x = attrib.normals[3 * idx.normal_index + 0];
                vertex.normal.y = attrib.normals[3 * idx.normal_index + 1];
                vertex.normal.z = attrib.normals[3 * idx.normal_index + 2];
            }

            if (idx.texcoord_index >= 0 && static_cast<size_t>(idx.texcoord_index) * 2 + 1 < attrib.texcoords.size()) {
                vertex.uv.x = attrib.texcoords[2 * idx.texcoord_index + 0];
                vertex.uv.y = attrib.texcoords[2 * idx.texcoord_index + 1];
            }

            mesh.vertices.push_back(vertex);
            mesh.indices.push_back(vertexOffset++);
        }
    }

    if (mesh.vertices.empty()) {
        ME_CORE_WARN("OBJ file '{0}' loaded but contains no vertices", path);
    }

    ME_CORE_INFO("Loaded OBJ '{0}': {1} vertices, {2} indices", mesh.name, mesh.vertices.size(), mesh.indices.size());

    return mesh;
}

}  // namespace Mantra
