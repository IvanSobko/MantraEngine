Some core principles and tips during the scene design phase:

1. Scene is just data, not behavior.
No systems, no update logic, no ECS.

It answers only one question:
“What exists in the world, and how does it look?”


2. Minimal Scene Structure

struct RenderScene {
    std::vector<Mesh> meshes;
    std::vector<Material> materials;
    std::vector<Texture> textures;
    std::vector<MeshInstance> instances;
    std::vector<Light> lights;
    Camera camera;
};

3. Mesh vs MeshInstance (Important Separation)

Mesh (shared geometry):
struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    BoundingBox bounds;
};
- raw geometry
- reused across many objects

MeshInstance (object in the scene):
struct MeshInstance {
    uint32_t meshID;
    uint32_t materialID;
    Transform transform;
};
- references mesh
- has transform
- has material

4. Data ownership
Until now the idea was that we upload textures, meshes to opengl(gpu), delete cpu data and be happy.
Now, since we will need a pbr renderer, we will need cpu copy of all the data. For now its fine, but i have to keep in mind that it can get out of hand
and be ready to optimize (load on demand, mip, texture streaming...).

The scene owns the data. The renderer derives GPU resources from it.
So all resouces must be loaded into scene and managed from the scene. Then we create gpu handles and upload data from the scene to them.