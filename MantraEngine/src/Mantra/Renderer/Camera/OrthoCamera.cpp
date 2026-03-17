#include "OrthoCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Mantra {

OrthoCamera::OrthoCamera(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
    mViewMatrix = glm::mat4(1.0f);
    mProjectionMatrix = glm::ortho(left, right, bottom, top, nearPlane, farPlane);
    mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
}

void OrthoCamera::SetProjection(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
    mProjectionMatrix = glm::ortho(left, right, bottom, top, nearPlane, farPlane);
    mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
}

void OrthoCamera::RecalculateViewMatrix() {
    // Create rotation matrices for pitch, yaw, and roll
    glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(mRotation.x), glm::vec3(1, 0, 0));  // Pitch
    glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), glm::radians(mRotation.y), glm::vec3(0, 1, 0));  // Yaw
    glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), glm::radians(mRotation.z), glm::vec3(0, 0, 1));  // Roll

    // Combine rotations (order matters: Z * Y * X for typical camera behavior)
    glm::mat4 rotation = rotationZ * rotationY * rotationX;

    // Create translation matrix
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), mPosition);

    // Combine transformation
    glm::mat4 transform = translation * rotation;

    mViewMatrix = glm::inverse(transform);
    mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
}

}  // namespace Mantra