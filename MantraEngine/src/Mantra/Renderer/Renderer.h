#pragma once

#include "Camera/Camera.h"
#include "RenderCommand.h"
#include "RendererAPI.h"
#include "Shader.h"
#include <cstdint>

namespace Mantra {

struct RenderStats {
    uint32_t drawCalls = 0;
    uint32_t verticesRendered = 0;
    uint32_t trianglesRendered = 0;
    float fps = 0.0f;
};

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
    
    static RenderStats GetFrameStats();
    static void ResetFrameStats();
    static void SetFrameTime(float deltaTime);

private:
    struct SceneData {
        glm::mat4 ViewProjectionMatrix;
        RenderStats FrameStats;
    };

    //TODO remove raw pointer
    static SceneData* mSceneData;
};

}  // namespace Mantra