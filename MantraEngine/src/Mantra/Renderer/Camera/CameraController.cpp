#include "CameraController.h"
#include "Mantra/Input/Input.h"
#include "Mantra/KeyCodes.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Mantra {

template <typename CameraType>
CameraController<CameraType>::CameraController(float aspectRatio) : mAspectRatio(aspectRatio) {
    if constexpr (std::is_same_v<CameraType, OrthoCamera>) {
        mCamera.SetProjection(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel);
    } else if constexpr (std::is_same_v<CameraType, PerspectiveCamera>) {
        mCamera.SetProjection(85.0f, mAspectRatio, 0.1f, 1000.0f);
    }
}

template <typename CameraType>
void CameraController<CameraType>::OnUpdate(Timestep ts) {
    glm::vec3 pos = mCamera.GetPosition();
    glm::vec3 rot = mCamera.GetRotation();
    float deltaTime = ts.GetSeconds();

    // maybe abolish controller and just have the camera handle input directly?

    if (Input::IsKeyPressed(ME_KEY_A)) {}
    if (Input::IsKeyPressed(ME_KEY_D)) {}

    MLOG("Camera position: ({:.2f}, {:.2f}, {:.2f})", pos.x, pos.y, pos.z);

    mCamera.SetRotation(rot);
    mCamera.SetPosition(pos);
}

template <typename CameraType>
void CameraController<CameraType>::OnEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseScrolledEvent>(std::bind(&CameraController::OnMouseScrolled, this, std::placeholders::_1));
    dispatcher.Dispatch<WindowResizeEvent>(std::bind(&CameraController::OnWindowResized, this, std::placeholders::_1));
}

template <typename CameraType>
bool CameraController<CameraType>::OnMouseScrolled(MouseScrolledEvent& e) {
    mZoomLevel -= e.GetYOffset() * 0.25f;
    mZoomLevel = std::max(mZoomLevel, 0.25f);
    SetZoomLevel(mZoomLevel);
    return false;
}

// Specialization for OrthoCamera
template <>
void CameraController<OrthoCamera>::SetZoomLevel(float level) {
    mZoomLevel = level;
    mCamera.SetProjection(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel);
}

// Specialization for PerspectiveCamera
template <>
void CameraController<PerspectiveCamera>::SetZoomLevel(float level) {
    mZoomLevel = level;
    // For perspective camera, zoom affects FOV (inverse relationship)
    float fov = 45.0f / level;            // Base FOV of 45 degrees
    fov = std::clamp(fov, 1.0f, 120.0f);  // Clamp FOV between 1 and 120 degrees
    mCamera.SetFOV(fov);
}

// Specialization for OrthoCamera window resize
template <>
bool CameraController<OrthoCamera>::OnWindowResized(WindowResizeEvent& e) {
    mAspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
    mCamera.SetProjection(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel);
    return false;
}

// Specialization for PerspectiveCamera window resize
template <>
bool CameraController<PerspectiveCamera>::OnWindowResized(WindowResizeEvent& e) {
    mAspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
    mCamera.SetAspectRatio(mAspectRatio);
    return false;
}

// Explicit template instantiations
template class CameraController<OrthoCamera>;
template class CameraController<PerspectiveCamera>;

}  // namespace Mantra