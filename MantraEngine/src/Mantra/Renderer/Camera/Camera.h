#pragma once

#include "glm/glm.hpp"

namespace Mantra {

class Camera
{
public:
    Camera() = default;
    virtual ~Camera() = default;

    // Common interface
    virtual const glm::vec3& GetPosition() const { return mPosition; }
    virtual void SetPosition(const glm::vec3& position);

    virtual const glm::vec3& GetRotation() const { return mRotation; }
    virtual void SetRotation(const glm::vec3& rotation);

    virtual const glm::mat4& GetProjectionMatrix() const { return mProjectionMatrix; }
    virtual const glm::mat4& GetViewMatrix() const { return mViewMatrix; }
    virtual const glm::mat4& GetViewProjectionMatrix() const { return mViewProjectionMatrix; }

protected:
    virtual void RecalculateViewMatrix() = 0;

protected:
    glm::mat4 mProjectionMatrix{1.0f};
    glm::mat4 mViewMatrix{1.0f};
    glm::mat4 mViewProjectionMatrix{1.0f};

    glm::vec3 mPosition = {0.0f, 0.0f, 0.0f};
    glm::vec3 mRotation = {0.0f, 0.0f, 0.0f};  // pitch, yaw, roll
};

}  // namespace Mantra