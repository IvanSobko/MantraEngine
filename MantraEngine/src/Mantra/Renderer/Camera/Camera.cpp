#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

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

}  // namespace Mantra