#pragma once

#include "Mantra/Renderer/Shader.h"

namespace Mantra {

class OpenGLShader : public Shader
{
public:
    OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
    ~OpenGLShader() override final;

    void Bind() const override final;
    void Unbind() const override final;

    void SetUniformInt(const std::string& name, int value);
    void SetUniformFloat(const std::string& name, float value);
    void SetUniformFloat2(const std::string& name, const glm::vec2& vector);
    void SetUniformFloat3(const std::string& name, const glm::vec3& vector);
    void SetUniformFloat4(const std::string& name, const glm::vec4& vector);

    void SetUniformMat3f(const std::string& name, const glm::mat3& matrix);
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:
    uint32_t mRendererID;

private:
};

}  // namespace Mantra