#include "Renderer.h"

#include "OpenGL/OpenGLShader.h"

namespace Mantra {

Renderer::SceneData* Renderer::mSceneData = new Renderer::SceneData;

void Renderer::Init() {
    RenderCommand::Init();
}

void Renderer::BeginScene(OrthoCamera& camera) {
    mSceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene() {}

void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray,
                      const glm::mat4& transform) {
    shader->Bind();
    // for now we assume that shader is OpenGLShader
    std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4f("u_ViewProjection",
                                                                     mSceneData->ViewProjectionMatrix);
    std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4f("u_Transform", transform);
    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
}

}  // namespace Mantra