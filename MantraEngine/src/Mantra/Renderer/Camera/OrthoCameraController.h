#pragma once

#include "Mantra/Events/ApplicationEvent.h"
#include "Mantra/Events/MouseEvent.h"
#include "Mantra/Renderer/Camera/Camera.h"
#include "Mantra/Renderer/Camera/OrthoCamera.h"
#include "Mantra/Renderer/Camera/PerspectiveCamera.h"
#include "Mantra/Timestep.h"

namespace Mantra {

template <typename CameraType>
class CameraController
{
public:
    CameraController(float aspectRatio);

    void OnUpdate(Timestep ts);
    void OnEvent(Event& e);

    CameraType& GetCamera() { return mCamera; }
    const CameraType& GetCamera() const { return mCamera; }

    float GetZoomLevel() const { return mZoomLevel; }
    void SetZoomLevel(float level);

private:
    bool OnMouseScrolled(MouseScrolledEvent& e);
    bool OnWindowResized(WindowResizeEvent& e);

private:
    float mAspectRatio;
    float mZoomLevel = 1.0f;
    CameraType mCamera;

    float mCameraTranslationSpeed = 5.0f;
    float mCameraRotationSpeed = 90.0f;
};

// Template specializations for zoom behavior
template <>
void CameraController<OrthoCamera>::SetZoomLevel(float level);

template <>
void CameraController<PerspectiveCamera>::SetZoomLevel(float level);

template <>
bool CameraController<OrthoCamera>::OnWindowResized(WindowResizeEvent& e);

template <>
bool CameraController<PerspectiveCamera>::OnWindowResized(WindowResizeEvent& e);

// Type aliases for convenience
using OrthoCameraController = CameraController<OrthoCamera>;
using PerspectiveCameraController = CameraController<PerspectiveCamera>;

}  // namespace Mantra