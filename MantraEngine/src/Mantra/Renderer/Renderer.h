#pragma once

#include "Camera/Camera.h"
#include "RenderCommand.h"
#include "RendererAPI.h"
#include "Shader.h"

namespace Mantra {

class Renderer
{
public:
    static void Init();
    static void BeginScene(Camera& camera);
    static void EndScene();

    static void Submit(const std::shared_ptr<Shader>& shader,
                       const std::shared_ptr<VertexArray>& vertexArrayvertexArray,
                       const glm::mat4& transform = glm::mat4(1.0f));

    inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

    static void OnWindowResize(uint32_t width, uint32_t height);

private:
    struct SceneData {
        glm::mat4 ViewProjectionMatrix;
    };

    //TODO remove raw pointer
    static SceneData* mSceneData;
};

}  // namespace Mantra