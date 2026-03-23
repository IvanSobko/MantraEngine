#include "OrthoCamera.h"

#include "Mantra/Events/ApplicationEvent.h"
#include "Mantra/Events/MouseEvent.h"
#include "Mantra/Input/Input.h"
#include "Mantra/KeyCodes.h"

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Mantra {

OrthoCamera::OrthoCamera(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
    mViewMatrix = glm::mat4(1.0f);
    mProjectionMatrix = glm::ortho(left, right, bottom, top, nearPlane, farPlane);
    mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
}

void OrthoCamera::UpdateViewMatrix() {
    glm::quat quatRotation = GetQuatRotation();
    mViewMatrix = glm::translate(glm::mat4(1.0f), mPosition) * glm::mat4_cast(quatRotation);
    // inverse because we want to move the world opposite to the camera's movement
    mViewMatrix = glm::inverse(mViewMatrix);
    mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
}

void OrthoCamera::UpdateProjectionMatrix() {
    mProjectionMatrix = glm::ortho(mProjectionBounds.x, mProjectionBounds.y, mProjectionBounds.z, mProjectionBounds.w,
                                   mNearPlane, mFarPlane);
    mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
}

void OrthoCamera::OnUpdate(float deltaTime) {
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
 
void OrthoCamera::OnEvent(Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<MouseScrolledEvent>(std::bind(&OrthoCamera::OnMouseScrolled, this, std::placeholders::_1));
    dispatcher.Dispatch<WindowResizeEvent>(std::bind(&OrthoCamera::OnWindowResized, this, std::placeholders::_1));
}

bool OrthoCamera::OnWindowResized(WindowResizeEvent& e) {
    MLOG("OrthoCamera: Window resized to {}x{}", e.GetWidth(), e.GetHeight());
    float aspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
    mProjectionBounds.x = -aspectRatio * mZoomLevel;
    mProjectionBounds.y = aspectRatio * mZoomLevel;

    UpdateProjectionMatrix();
    return true;
}

bool OrthoCamera::OnMouseScrolled(MouseScrolledEvent& e) {
    float scrollAmount = e.GetYOffset() * 0.1f;
    mPosition += GetForwardDirection() * scrollAmount;
    mZoomLevel = std::clamp(mZoomLevel - scrollAmount, 0.1f, 100.0f);

    //TODO: add proper aspect ratio handling here instead of hardcoding 1.7f
    float halfWidth = 1.7f * mZoomLevel;
    float halfHeight = mZoomLevel;

    mProjectionBounds = {-halfWidth, halfWidth, -halfHeight, halfHeight};

    UpdateProjectionMatrix();
    return false;
}

void OrthoCamera::ResetView() {
    mPosition = {0.0f, 0.0f, 0.0f};
    mRotation = {0.0f, 0.0f, 0.0f};
    mZoomLevel = 1.0f;
    mProjectionBounds.z = -mZoomLevel;
    mProjectionBounds.w = mZoomLevel;
    UpdateProjectionMatrix();
    UpdateViewMatrix();
}

}  // namespace Mantra