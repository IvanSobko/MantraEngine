#include "Renderer.h"

namespace Mantra {

Renderer::SceneData* Renderer::mSceneData = new Renderer::SceneData;

void Renderer::BeginScene(OrthoCamera& camera) {
    mSceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene() {}

void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray) {
    shader->Bind();
    shader->SetUniformMat4f("u_ViewProjection", mSceneData->ViewProjectionMatrix);
    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
}

}  // namespace Mantra