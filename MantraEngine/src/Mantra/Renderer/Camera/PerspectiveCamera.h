#pragma once

#include "Camera.h"

namespace Mantra {

class MouseScrolledEvent;
class WindowResizeEvent;

class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera() = default;
    PerspectiveCamera(float fov, float aspectRatio, float nearPlane, float farPlane);

    float GetFOV() const { return mFOV; }
    void SetFOV(float fov);

    float GetAspectRatio() const { return mAspectRatio; }
    void SetAspectRatio(float aspectRatio);

    void SetPosition(const glm::vec3& position) override;
    void SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane);

    void OnUpdate(float deltaTime) override;
    void OnEvent(Event& event) override;

    void ResetView() override;

protected:
    void UpdateViewMatrix() override;
    void UpdateProjectionMatrix() override;

private:
    bool OnWindowResized(WindowResizeEvent& e);
    bool OnMouseScrolled(MouseScrolledEvent& e);

    glm::vec3 CalculatePosition() const;

    glm::vec3 GetUpDirection() const;
    glm::vec3 GetRightDirection() const;
    glm::vec3 GetForwardDirection() const;

    float mFOV = 45.0f;
    float mAspectRatio = 16.0f / 9.0f;
    float mNearPlane = 0.1f;
    float mFarPlane = 1000.0f;

    glm::vec3 mFocalPoint = {0.0f, 0.0f, 0.0f};
    glm::vec2 mPrevMousePosition = {0.0f, 0.0f};

    float mDistance = 10.0f;
};

}  // namespace Mantra