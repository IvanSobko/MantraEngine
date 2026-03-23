#pragma once

#include "PerspectiveCamera.h"

namespace Mantra {

class MouseScrolledEvent;
class WindowResizeEvent;

/*
Idea is to to have a camera that orbits around a focal point rather than full 6dof movement.
*/

class EditorCamera : public PerspectiveCamera
{
public:
    EditorCamera() = default;
    EditorCamera(float fov, float aspectRatio, float nearPlane, float farPlane);

    void SetPosition(const glm::vec3& position) override;
    void OnUpdate(float deltaTime) override;
    void ResetView() override;

protected:
    void UpdateViewMatrix() override;

    bool OnWindowResized(WindowResizeEvent& e);
    bool OnMouseScrolled(MouseScrolledEvent& e) override;

private:
    glm::vec3 CalculatePosition() const;

    float mDistance = 10.0f;
    glm::vec3 mFocalPoint = {0.0f, 0.0f, 0.0f};
    glm::vec2 mPrevMousePosition = {0.0f, 0.0f};
};

}  // namespace Mantra