#include "PerspectiveCamera.h"

#include "Mantra/Events/ApplicationEvent.h"
#include "Mantra/Events/MouseEvent.h"
#include "Mantra/Input/Input.h"
#include "Mantra/KeyCodes.h"

#include "glm/gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Mantra {

PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearPlane, float farPlane)
    : mFOV(fov), mAspectRatio(aspectRatio), mNearPlane(nearPlane), mFarPlane(farPlane) {
    UpdateProjectionMatrix();
    UpdateViewMatrix();
}

void PerspectiveCamera::UpdateViewMatrix() {
    glm::quat orientation = GetQuatRotation();
    mViewMatrix = glm::translate(glm::mat4(1.0f), mPosition) * glm::mat4_cast(orientation);
    mViewMatrix = glm::inverse(mViewMatrix);
    mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
}

void PerspectiveCamera::UpdateProjectionMatrix() {
    mProjectionMatrix = glm::perspective(glm::radians(mFOV), mAspectRatio, mNearPlane, mFarPlane);
    mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
}

void PerspectiveCamera::OnUpdate(float deltaTime) {
    if (Input::IsKeyPressed(ME_KEY_LEFT_ALT)) {
        auto [x, y] = Input::GetMousePosition();
        const glm::vec2& mouse{x, y};
        glm::vec2 delta = (mouse - mPrevMousePosition) * 0.003f;
        mPrevMousePosition = mouse;

        if (Input::IsMouseButtonPressed(ME_MOUSE_BUTTON_RIGHT)) {
            mPosition += -GetRightDirection() * delta.x;
            mPosition += GetUpDirection() * delta.y;
        } else if (Input::IsMouseButtonPressed(ME_MOUSE_BUTTON_LEFT)) {
            float rotationSpeed = 0.8f;
            float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
            mRotation.y += yawSign * delta.x * rotationSpeed;
            mRotation.x += delta.y * rotationSpeed;
        }
    }

    UpdateViewMatrix();
}

void PerspectiveCamera::OnEvent(Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<MouseScrolledEvent>(
        std::bind(&PerspectiveCamera::OnMouseScrolled, this, std::placeholders::_1));
    dispatcher.Dispatch<WindowResizeEvent>(std::bind(&PerspectiveCamera::OnWindowResized, this, std::placeholders::_1));
}

bool PerspectiveCamera::OnWindowResized(WindowResizeEvent& e) {
    mAspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
    UpdateProjectionMatrix();
    return true;
}

bool PerspectiveCamera::OnMouseScrolled(MouseScrolledEvent& e) {
    float scrollAmount = e.GetYOffset() * 0.1f;
    mPosition += GetForwardDirection() * scrollAmount;

    UpdateViewMatrix();
    return false;
}

void PerspectiveCamera::ResetView() {
    mPosition = {0.0f, 0.0f, 0.0f};
    mRotation = {0.0f, 0.0f, 0.0f};
    UpdateViewMatrix();
}

}  // namespace Mantra