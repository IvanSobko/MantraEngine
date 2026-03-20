#pragma once

#include "Camera.h"

namespace Mantra {

class OrthoCamera : public Camera
{
public:
    OrthoCamera() = default;
    OrthoCamera(float left, float right, float bottom, float top, float nearPlane = -100.0f, float farPlane = 100.0f);

    void SetProjection(float left, float right, float bottom, float top, float nearPlane = -100.0f,
                       float farPlane = 100.0f);

protected:
    void UpdateProjectionMatrix() override;
    void UpdateViewMatrix() override;

private:
    glm::vec4 mProjectionBounds{0.0f, 0.0f, 0.0f, 0.0f};  // left, right, bottom, top
    float mNearPlane = -100.0f;
    float mFarPlane = 100.0f;
};

}  // namespace Mantra