#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Mantra {

void Camera::SetPosition(const glm::vec3& position) {
    mPosition = position;
    RecalculateViewMatrix();
}

void Camera::SetRotation(const glm::vec3& rotation) {
    mRotation = rotation;
    RecalculateViewMatrix();
}

}  // namespace Mantra