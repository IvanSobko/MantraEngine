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
}

void Renderer::EndScene() {}

void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray,
                      const glm::mat4& transform) {
    shader->Bind();
    shader->SetUniformMat4f("u_ViewProjection", mSceneData->ViewProjectionMatrix);
    shader->SetUniformMat4f("u_Transform", transform);
    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
}

void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
    RenderCommand::SetViewport(0, 0, width, height);
}

}  // namespace Mantra