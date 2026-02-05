#pragma once

#include "Cameras/OrthoCamera.h"
#include "RenderCommand.h"
#include "RendererAPI.h"
#include "Shader.h"

namespace Mantra {

class Renderer
{
public:
    static void BeginScene(OrthoCamera& camera);
    static void EndScene();

    static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

    inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

private:
    struct SceneData {
        glm::mat4 ViewProjectionMatrix;
    };

    static SceneData* mSceneData;
};

}  // namespace Mantra