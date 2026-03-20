#include "OrthoCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Mantra {

OrthoCamera::OrthoCamera(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
    mViewMatrix = glm::mat4(1.0f);
    mProjectionMatrix = glm::ortho(left, right, bottom, top, nearPlane, farPlane);
    mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
}

void OrthoCamera::SetProjection(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
    mProjectionBounds = {left, right, bottom, top};
    mNearPlane = nearPlane;
    mFarPlane = farPlane;
    UpdateProjectionMatrix();
}

void OrthoCamera::UpdateViewMatrix() {
    glm::quat quatRotation = GetRotation();
    glm::mat4 rotation = glm::mat4_cast(quatRotation);

    // Create translation matrix
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), mPosition);

    // Combine transformation
    glm::mat4 transform = translation * rotation;

    // inverse because we want to move the world opposite to the camera's movement
    mViewMatrix = glm::inverse(transform);
    mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
}

void OrthoCamera::UpdateProjectionMatrix() {
    mProjectionMatrix = glm::ortho(mProjectionBounds.x, mProjectionBounds.y, mProjectionBounds.z, mProjectionBounds.w,
                                   mNearPlane, mFarPlane);
    mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
}

}  // namespace Mantra