#include "OrthoCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Mantra {

OrthoCamera::OrthoCamera(float left, float right, float bottom, float top) {
    mViewMatrix = glm::mat4(1.0f);
    mProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
}

void OrthoCamera::SetPosition(const glm::vec3& position) {
    mPosition = position;
    RecalculateViewMatrix();
}

void OrthoCamera::SetRotation(float rotation) {
    mRotation = rotation;
    RecalculateViewMatrix();
}

void OrthoCamera::RecalculateViewMatrix() {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), mPosition) *
                          glm::rotate(glm::mat4(1.0f), glm::radians(mRotation), glm::vec3(0, 0, 1));

    mViewMatrix = glm::inverse(transform);
    mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
}

}  // namespace Mantra