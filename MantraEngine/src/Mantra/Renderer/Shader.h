#pragma once

#include <glm/glm.hpp>
#include <string>

namespace Mantra {

class Shader
{
public:
    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    // name is optional, if not provided, it will be extracted from the filepath
    static Shader* Create(const std::string& filepath, const std::string& name = "");
    static Shader* Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

    const std::string& GetName() const { return mName; }

protected:
    std::string mName;
};

class ShaderLibrary
{
public:
    ShaderLibrary() = default;
    ~ShaderLibrary();

    void Add(const std::string& name, const std::shared_ptr<Shader>& shader);
    void Add(const std::shared_ptr<Shader>& shader);
    std::shared_ptr<Shader> Load(const std::string& filepath);
    std::shared_ptr<Shader> Load(const std::string& name, const std::string& filepath);

    std::shared_ptr<Shader> Get(const std::string& name);

    bool Exists(const std::string& name) const;

private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> mShaders;
};

}  // namespace Mantra