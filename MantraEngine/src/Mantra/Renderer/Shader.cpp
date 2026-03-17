#include "Shader.h"

#include "OpenGL/OpenGLShader.h"
#include "Renderer.h"

namespace Mantra {

std::shared_ptr<Shader> Shader::Create(const std::string& filepath, const std::string& name) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            ME_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(filepath, name);
    }

    ME_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

std::shared_ptr<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc,
                                       const std::string& fragmentSrc) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            ME_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
    }

    ME_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

ShaderLibrary::~ShaderLibrary() {
    mShaders.clear();
}

void ShaderLibrary::Add(const std::string& name, const std::shared_ptr<Shader>& shader) {
    if (Exists(name)) {
        ME_CORE_ERROR("Shader '{0}' already exists in library!", name);
        return;
    }
    mShaders[name] = shader;
}

void ShaderLibrary::Add(const std::shared_ptr<Shader>& shader) {
    Add(shader->GetName(), shader);
}

std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& filepath) {
    std::shared_ptr<Shader> shader(Shader::Create(filepath));
    Add(shader);
    return shader;
}

std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath) {
    std::shared_ptr<Shader> shader(Shader::Create(filepath, name));
    Add(shader);
    return shader;
}

std::shared_ptr<Shader> ShaderLibrary::Get(const std::string& name) {
    if (!Exists(name)) {
        ME_CORE_ERROR("Shader '{0}' not found in library!", name);
        return nullptr;
    }
    return mShaders[name];
}

bool ShaderLibrary::Exists(const std::string& name) const {
    return mShaders.find(name) != mShaders.end();
}

}  // namespace Mantra