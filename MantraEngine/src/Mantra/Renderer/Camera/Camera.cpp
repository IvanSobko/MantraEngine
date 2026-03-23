#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Mantra {

void Camera::SetPosition(const glm::vec3& position) {
    mPosition = position;
    UpdateViewMatrix();
}

void Camera::SetRotation(const glm::vec3& rotationRad) {
    glm::vec3 rotationDeg = glm::degrees(rotationRad);
    MLOG("Setting Euler rotation: ({}, {}, {})", rotationDeg.x, rotationDeg.y, rotationDeg.z);
    mRotation = rotationRad;
    UpdateViewMatrix();
}

void Camera::ResetView() {
    mPosition = {0.0f, 0.0f, 0.0f};
    mRotation = {0.0f, 0.0f, 0.0f};
    UpdateViewMatrix();
}

glm::vec3 Camera::GetUpDirection() const {
    return glm::rotate(GetQuatRotation(), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 Camera::GetRightDirection() const {
    return glm::rotate(GetQuatRotation(), glm::vec3(1.0f, 0.0f, 0.0f));
}

glm::vec3 Camera::GetForwardDirection() const {
    return glm::rotate(GetQuatRotation(), glm::vec3(0.0f, 0.0f, -1.0f));
}

}  // namespace Mantra