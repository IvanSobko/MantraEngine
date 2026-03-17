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
    void RecalculateViewMatrix() override;
};

}  // namespace Mantra