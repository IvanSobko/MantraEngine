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

    void OnUpdate(float deltaTime) override;
    void OnEvent(Event& event) override;
    void ResetView() override;
    void SetViewportSize(uint32_t width, uint32_t height) override;

protected:
    void UpdateViewMatrix() override;
    void UpdateProjectionMatrix() override;

    virtual bool OnMouseScrolled(MouseScrolledEvent& e);

private:
    float mFOV = 60.0f;
    float mAspectRatio = 16.0f / 9.0f;
    float mNearPlane = 0.1f;
    float mFarPlane = 1000.0f;

    glm::vec2 mPrevMousePosition = {0.0f, 0.0f};
};

}  // namespace Mantra