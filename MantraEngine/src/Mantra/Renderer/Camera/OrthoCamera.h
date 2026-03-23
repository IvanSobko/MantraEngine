#pragma once

#include "Camera.h"

namespace Mantra {

class MouseScrolledEvent;
class WindowResizeEvent;

class OrthoCamera : public Camera
{
public:
    OrthoCamera() = default;
    OrthoCamera(float left, float right, float bottom, float top, float nearPlane = -100.0f, float farPlane = 100.0f);

    void OnUpdate(float deltaTime) override;
    void OnEvent(Event& event) override;
    void ResetView() override;

protected:
    void UpdateProjectionMatrix() override;
    void UpdateViewMatrix() override;

    bool OnWindowResized(WindowResizeEvent& e);
    virtual bool OnMouseScrolled(MouseScrolledEvent& e);

private:
    glm::vec4 mProjectionBounds{0.0f, 0.0f, 0.0f, 0.0f};  // left, right, bottom, top
    float mNearPlane = -100.0f;
    float mFarPlane = 100.0f;
    float mZoomLevel = 1.0f;

    glm::vec2 mPrevMousePosition = {0.0f, 0.0f};
};

}  // namespace Mantra