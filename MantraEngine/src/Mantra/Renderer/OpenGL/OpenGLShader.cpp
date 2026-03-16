#include "OpenGLShader.h"

#include "Mantra/Core.h"

#include <glad/glad.h>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>

namespace Mantra {

const char* TYPE_TOKEN = "#me_define";

static GLenum ShaderTypeFromString(const std::string& type) {
    if (type == "vertex")
        return GL_VERTEX_SHADER;
    if (type == "fragment" || type == "pixel")
        return GL_FRAGMENT_SHADER;
    if (type == "geometry")
        return GL_GEOMETRY_SHADER;
    if (type == "compute")
        return GL_COMPUTE_SHADER;

    ME_CORE_ERROR("Unknown shader type specified: '{0}'", type);
    ME_CORE_ASSERT(false, "Unknown shader type");
    return 0;
}

OpenGLShader::OpenGLShader(const std::string& filepath, const std::string& name) {
    // Load shader from file
    std::ifstream file(filepath);

    if (!file.is_open()) {
        ME_CORE_ERROR("Failed to open shader file: {0}", filepath);
        return;
    }

    std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    if (name.empty()) {  // Extract name from filepath
        size_t lastSlash = filepath.find_last_of("/\\");
        size_t lastDot = filepath.find_last_of('.');
        mName = filepath.substr(lastSlash + 1, lastDot - lastSlash - 1);
    } else {
        mName = name;
    }

    // Preprocess shader source to separate different shader stages
    std::unordered_map<GLenum, std::string> shaderSources;
    size_t pos = source.find(TYPE_TOKEN, 0);
    while (pos != std::string::npos) {
        size_t eol = source.find_first_of("\r\n", pos);
        ME_CORE_ASSERT(eol != std::string::npos, "Syntax error in shader file!");
        size_t begin = pos + strlen(TYPE_TOKEN) + 1;
        std::string type = source.substr(begin, eol - begin);
        type.erase(std::remove_if(type.begin(), type.end(), ::isspace), type.end());
        ME_CORE_ASSERT(!type.empty(), "Shader type not specified!");

        size_t nextLinePos = source.find_first_not_of("\r\n", eol);
        pos = source.find(TYPE_TOKEN, nextLinePos);
        shaderSources[ShaderTypeFromString(type)] =
            source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
    }

    std::vector<GLuint> shaderIDs = Compile(shaderSources);
    ME_CORE_ASSERT(!shaderIDs.empty(), "Shader compilation failed!");
    bool linkResult = Link(shaderIDs);
    ME_CORE_ASSERT(linkResult, "Shader linking failed!");
}

OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) {
    mName = name;

    std::unordered_map<GLenum, std::string> sources;
    sources[GL_VERTEX_SHADER] = vertexSrc;
    sources[GL_FRAGMENT_SHADER] = fragmentSrc;

    std::vector<GLuint> shaderIDs = Compile(sources);
    ME_CORE_ASSERT(!shaderIDs.empty(), "Shader compilation failed!");
    bool linkResult = Link(shaderIDs);
    ME_CORE_ASSERT(linkResult, "Shader linking failed!");
}

OpenGLShader::~OpenGLShader() {
    glDeleteProgram(mRendererID);
}

std::vector<GLuint> OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources) {
    mRendererID = glCreateProgram();
    std::vector<GLuint> glShaderIDs;

    for (const auto& kv : shaderSources) {
        GLenum type = kv.first;
        const std::string& source = kv.second;

        GLuint shader = glCreateShader(type);
        const GLchar* sourceCStr = source.c_str();
        glShaderSource(shader, 1, &sourceCStr, 0);
        glCompileShader(shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

            // Clean up on failure
            glDeleteShader(shader);
            for (GLuint id : glShaderIDs) {
                glDeleteShader(id);
            }
            glDeleteProgram(mRendererID);

            ME_CORE_ERROR("Shader compile failed: {0}", infoLog.data());
            return {};
        }

        glAttachShader(mRendererID, shader);
        glShaderIDs.push_back(shader);
    }

    return glShaderIDs;
}

bool OpenGLShader::Link(const std::vector<GLuint>& shaderIDs) {
    if (shaderIDs.empty()) {
        ME_CORE_ERROR("No shaders to link!");
        return false;
    }

    glLinkProgram(mRendererID);

    GLint isLinked = 0;
    glGetProgramiv(mRendererID, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(mRendererID, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(mRendererID, maxLength, &maxLength, &infoLog[0]);

        // Clean up on failure
        glDeleteProgram(mRendererID);
        mRendererID = 0;

        for (auto id : shaderIDs) {
            glDeleteShader(id);
        }

        ME_CORE_ERROR("Shader link failed: {0}", infoLog.data());
        return false;
    }

    // Detach and delete shaders after successful link
    for (GLuint id : shaderIDs) {
        glDetachShader(mRendererID, id);
        glDeleteShader(id);
    }

    return true;
}

void OpenGLShader::Bind() const {
    glUseProgram(mRendererID);
}

void OpenGLShader::Unbind() const {
    glUseProgram(0);
}

void OpenGLShader::SetUniformInt(const std::string& name, int value) {
    GLint location = glGetUniformLocation(mRendererID, name.c_str());
    glUniform1i(location, value);
}

void OpenGLShader::SetUniformFloat(const std::string& name, float value) {
    GLint location = glGetUniformLocation(mRendererID, name.c_str());
    glUniform1f(location, value);
}

void OpenGLShader::SetUniformFloat2(const std::string& name, const glm::vec2& vector) {
    GLint location = glGetUniformLocation(mRendererID, name.c_str());
    glUniform2f(location, vector.x, vector.y);
}

void OpenGLShader::SetUniformFloat3(const std::string& name, const glm::vec3& vector) {
    GLint location = glGetUniformLocation(mRendererID, name.c_str());
    glUniform3f(location, vector.x, vector.y, vector.z);
}

void OpenGLShader::SetUniformFloat4(const std::string& name, const glm::vec4& vector) {
    GLint location = glGetUniformLocation(mRendererID, name.c_str());
    glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
}

void OpenGLShader::SetUniformMat3f(const std::string& name, const glm::mat3& matrix) {
    GLint location = glGetUniformLocation(mRendererID, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
    GLint location = glGetUniformLocation(mRendererID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

}  // namespace Mantra