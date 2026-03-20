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

void PerspectiveCamera::SetFOV(float fov) {
    mFOV = fov;
    UpdateProjectionMatrix();
}

void PerspectiveCamera::SetAspectRatio(float aspectRatio) {
    mAspectRatio = aspectRatio;
    UpdateProjectionMatrix();
}

void PerspectiveCamera::SetPosition(const glm::vec3& position) {
    // mFocalPoint = position; or
    // mPosition = position; ??
    // UpdateViewMatrix();
    // ignore for now, since idk if i want to handle direct position directly
}

void PerspectiveCamera::SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane) {
    mFOV = fov;
    mAspectRatio = aspectRatio;
    mNearPlane = nearPlane;
    mFarPlane = farPlane;
    UpdateProjectionMatrix();
}

void PerspectiveCamera::UpdateViewMatrix() {

    mPosition = CalculatePosition();

    glm::quat orientation = GetQuatRotation();
    mViewMatrix = glm::translate(glm::mat4(1.0f), mPosition) * glm::toMat4(orientation);
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

        if (Input::IsMouseButtonPressed(ME_MOUSE_BUTTON_MIDDLE)) {
            mFocalPoint += -GetRightDirection() * delta.x * mDistance;
            mFocalPoint += GetUpDirection() * delta.y * mDistance;
        } else if (Input::IsMouseButtonPressed(ME_MOUSE_BUTTON_LEFT)) {
            float rotationSpeed = 0.8f;
            float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;

            mRotation.y += yawSign * delta.x * rotationSpeed;
            mRotation.x += delta.y * rotationSpeed;
        }

        MLOG("Camera focal point: ({:.2f}, {:.2f}, {:.2f})", mFocalPoint.x, mFocalPoint.y, mFocalPoint.z);
        MLOG("Camera distance: {:.2f}", mDistance);
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
    float speed = std::min(mDistance, 50.0f);  // max speed = 50

    mDistance -= e.GetYOffset() * 0.1f * speed;
    mDistance = std::max(mDistance, 0.1f);  // prevent flipping through the focal point

    UpdateViewMatrix();
    return false;
}

void PerspectiveCamera::ResetView() {
    mFocalPoint = {0.0f, 0.0f, 0.0f};
    mDistance = 10.0f;
    mRotation = {0.0f, 0.0f, 0.0f};
    UpdateViewMatrix();
}

glm::vec3 PerspectiveCamera::CalculatePosition() const {
    return mFocalPoint - GetForwardDirection() * mDistance;
}

glm::vec3 PerspectiveCamera::GetUpDirection() const {
    return glm::rotate(GetQuatRotation(), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 PerspectiveCamera::GetRightDirection() const {
    return glm::rotate(GetQuatRotation(), glm::vec3(1.0f, 0.0f, 0.0f));
}

glm::vec3 PerspectiveCamera::GetForwardDirection() const {
    return glm::rotate(GetQuatRotation(), glm::vec3(0.0f, 0.0f, -1.0f));
}

}  // namespace Mantra