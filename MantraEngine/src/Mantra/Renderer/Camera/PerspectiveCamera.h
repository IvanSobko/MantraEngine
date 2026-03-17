#pragma once

#include "Camera.h"

namespace Mantra {

class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera() = default;
    PerspectiveCamera(float fov, float aspectRatio, float nearPlane, float farPlane);

    float GetFOV() const { return mFOV; }
    void SetFOV(float fov);

    float GetAspectRatio() const { return mAspectRatio; }
    void SetAspectRatio(float aspectRatio);

    void SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane);

protected:
    void RecalculateViewMatrix() override;
    void RecalculateProjectionMatrix();

private:
    float mFOV = 45.0f;
    float mAspectRatio = 16.0f / 9.0f;
    float mNearPlane = 0.1f;
    float mFarPlane = 1000.0f;
};

}  // namespace Mantra