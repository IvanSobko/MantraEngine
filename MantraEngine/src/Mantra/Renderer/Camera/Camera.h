#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

namespace Mantra {

class Event;

class Camera
{
public:
    Camera() = default;
    virtual ~Camera() = default;

    // Common interface
    virtual const glm::vec3& GetPosition() const { return mPosition; }
    virtual void SetPosition(const glm::vec3& position);

    virtual glm::vec3 GetRotation() const { return mRotation; }
    virtual glm::quat GetQuatRotation() const { return glm::quat(glm::vec3(-mRotation.x, -mRotation.y, mRotation.z)); }
    virtual void SetRotation(const glm::vec3& rotationRad);

    virtual const glm::mat4& GetProjectionMatrix() const { return mProjectionMatrix; }
    virtual const glm::mat4& GetViewMatrix() const { return mViewMatrix; }
    virtual const glm::mat4& GetViewProjectionMatrix() const { return mViewProjectionMatrix; }

    virtual void OnUpdate(float deltaTime) {}
    virtual void OnEvent(Event& event) {}
    virtual void SetViewportSize(uint32_t width, uint32_t height);

    virtual void ResetView();

protected:
    virtual void UpdateViewMatrix() = 0;
    virtual void UpdateProjectionMatrix() = 0;

    glm::vec3 GetUpDirection() const;
    glm::vec3 GetRightDirection() const;
    glm::vec3 GetForwardDirection() const;

protected:
    glm::mat4 mProjectionMatrix{1.0f};
    glm::mat4 mViewMatrix{1.0f};
    glm::mat4 mViewProjectionMatrix{1.0f};

    glm::vec3 mPosition = {0.0f, 0.0f, 0.0f};
    glm::vec3 mRotation = {0.0f, 0.0f, 0.0f};  // pitch, yaw, roll

    glm::vec2 mViewportSize = {1280.0f, 720.0f};
};

}  // namespace Mantra