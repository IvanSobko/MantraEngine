#pragma once

#include "glm/glm.hpp"

namespace Mantra {

class OrthoCamera
{
public:
    OrthoCamera() = default;
    OrthoCamera(float left, float right, float bottom, float top);

    const glm::vec3& GetPosition() const { return mPosition; }
    void SetPosition(const glm::vec3& position);

    float GetRotation() const { return mRotation; }
    void SetRotation(float rotation);

    void SetProjection(float left, float right, float bottom, float top);

    const glm::mat4& GetProjectionMatrix() const { return mProjectionMatrix; }
    const glm::mat4& GetViewMatrix() const { return mViewMatrix; }
    const glm::mat4& GetViewProjectionMatrix() const { return mViewProjectionMatrix; }

private:
    void RecalculateViewMatrix();

private:
    glm::mat4 mProjectionMatrix;
    glm::mat4 mViewMatrix;
    glm::mat4 mViewProjectionMatrix;

    glm::vec3 mPosition = {0.0f, 0.0f, 0.0f};
    float mRotation = 0.0f;  // 1d rotation around the Z axis for now
};

}  // namespace Mantra