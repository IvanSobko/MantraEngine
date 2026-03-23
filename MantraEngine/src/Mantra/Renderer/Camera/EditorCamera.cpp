#include "EditorCamera.h"

#include "Mantra/Events/ApplicationEvent.h"
#include "Mantra/Events/MouseEvent.h"
#include "Mantra/Input/Input.h"
#include "Mantra/KeyCodes.h"

#include "glm/gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Mantra {

EditorCamera::EditorCamera(float fov, float aspectRatio, float nearPlane, float farPlane)
    : PerspectiveCamera(fov, aspectRatio, nearPlane, farPlane) {
    UpdateProjectionMatrix();
    UpdateViewMatrix();
}

void EditorCamera::SetPosition(const glm::vec3& position) {
    // mFocalPoint = position; or
    // mPosition = position; ??
    // UpdateViewMatrix();
    // ignore for now, since idk if i want to handle position directly
}

void EditorCamera::UpdateViewMatrix() {
    mPosition = CalculatePosition();
    PerspectiveCamera::UpdateViewMatrix();
}

void EditorCamera::OnUpdate(float deltaTime) {
    if (Input::IsKeyPressed(ME_KEY_LEFT_ALT)) {
        auto [x, y] = Input::GetMousePosition();
        const glm::vec2& mouse{x, y};
        glm::vec2 delta = (mouse - mPrevMousePosition) * 0.003f;
        mPrevMousePosition = mouse;

        if (Input::IsMouseButtonPressed(ME_MOUSE_BUTTON_RIGHT)) {
            mFocalPoint += -GetRightDirection() * delta.x * mDistance;
            mFocalPoint += GetUpDirection() * delta.y * mDistance;
        } else if (Input::IsMouseButtonPressed(ME_MOUSE_BUTTON_LEFT)) {
            float rotationSpeed = 0.8f;
            float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
            mRotation.y += yawSign * delta.x * rotationSpeed;
            mRotation.x += delta.y * rotationSpeed;
        }
    }

    UpdateViewMatrix();
}

bool EditorCamera::OnMouseScrolled(MouseScrolledEvent& e) {
    float speed = std::min(mDistance, 50.0f);  // max speed = 50

    mDistance -= e.GetYOffset() * 0.1f * speed;
    mDistance = std::clamp(mDistance, 0.1f, 1000.0f);  // prevent flipping through the focal point

    UpdateViewMatrix();
    return false;
}

void EditorCamera::ResetView() {
    mFocalPoint = {0.0f, 0.0f, 0.0f};
    mDistance = 10.0f;
    mRotation = {0.0f, 0.0f, 0.0f};
    UpdateViewMatrix();
}

glm::vec3 EditorCamera::CalculatePosition() const {
    return mFocalPoint - GetForwardDirection() * mDistance;
}

}  // namespace Mantra