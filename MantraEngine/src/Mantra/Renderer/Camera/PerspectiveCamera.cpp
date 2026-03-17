#include "PerspectiveCamera.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Mantra {

PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearPlane, float farPlane)
    : mFOV(fov), mAspectRatio(aspectRatio), mNearPlane(nearPlane), mFarPlane(farPlane) {
    RecalculateProjectionMatrix();
    RecalculateViewMatrix();
}

void PerspectiveCamera::SetFOV(float fov) {
    mFOV = fov;
    RecalculateProjectionMatrix();
}

void PerspectiveCamera::SetAspectRatio(float aspectRatio) {
    mAspectRatio = aspectRatio;
    RecalculateProjectionMatrix();
}

void PerspectiveCamera::SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane) {
    mFOV = fov;
    mAspectRatio = aspectRatio;
    mNearPlane = nearPlane;
    mFarPlane = farPlane;
    RecalculateProjectionMatrix();
}

void PerspectiveCamera::RecalculateViewMatrix() {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), mPosition);

    // Apply rotations in order: pitch (X), yaw (Y), roll (Z)
    transform = glm::rotate(transform, glm::radians(mRotation.x), glm::vec3(1, 0, 0));
    transform = glm::rotate(transform, glm::radians(mRotation.y), glm::vec3(0, 1, 0));
    transform = glm::rotate(transform, glm::radians(mRotation.z), glm::vec3(0, 0, 1));

    mViewMatrix = glm::inverse(transform);
    mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
}

void PerspectiveCamera::RecalculateProjectionMatrix() {
    mProjectionMatrix = glm::perspective(glm::radians(mFOV), mAspectRatio, mNearPlane, mFarPlane);
    mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
}

}  // namespace Mantra