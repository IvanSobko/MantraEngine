#include "Renderer.h"

#include "OpenGL/OpenGLShader.h"

namespace Mantra {

Renderer::SceneData* Renderer::mSceneData = new Renderer::SceneData;

void Renderer::Init() {
    RenderCommand::Init();
}

//TODO: make camera const
void Renderer::BeginScene(Camera& camera) {
    mSceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    ResetFrameStats();
}

void Renderer::EndScene() {}

void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray,
                      const glm::mat4& transform) {
    shader->Bind();
    shader->SetUniformMat4f("u_ViewProjection", mSceneData->ViewProjectionMatrix);
    shader->SetUniformMat4f("u_Transform", transform);
    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);

    // Track stats
    auto indexBuffer = vertexArray->GetIndexBuffer();
    if (indexBuffer) {
        uint32_t indexCount = indexBuffer->GetCount();
        mSceneData->FrameStats.drawCalls++;
        mSceneData->FrameStats.verticesRendered += indexCount;
        mSceneData->FrameStats.trianglesRendered += indexCount / 3;
    }
}

void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
    RenderCommand::SetViewport(0, 0, width, height);
}

RenderStats Renderer::GetFrameStats() {
    return mSceneData->FrameStats;
}

void Renderer::ResetFrameStats() {
    mSceneData->FrameStats = RenderStats();
}

void Renderer::SetFrameTime(float deltaTime) {
    if (deltaTime > 0.0f) {
        mSceneData->FrameStats.fps = 1.0f / deltaTime;
    }
}

}  // namespace Mantra