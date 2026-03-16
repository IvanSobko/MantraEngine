#pragma once

#include "Mantra/Events/ApplicationEvent.h"
#include "Mantra/Events/MouseEvent.h"
#include "Mantra/Renderer/Cameras/OrthoCamera.h"
#include "Mantra/Timestep.h"

namespace Mantra {

class OrthoCameraController
{
public:
    OrthoCameraController(float aspectRatio, bool rotation = false);

    void OnUpdate(Timestep ts);
    void OnEvent(Event& e);

    OrthoCamera& GetCamera() { return mCamera; }
    const OrthoCamera& GetCamera() const { return mCamera; }

private:
    bool OnMouseScrolled(MouseScrolledEvent& e);
    bool OnWindowResized(WindowResizeEvent& e);

private:
    float mAspectRatio;
    float mZoomLevel = 1.0f;
    OrthoCamera mCamera;

    bool mRotation;

    glm::vec3 mCameraPosition = {0.0f, 0.0f, 0.0f};
    float mCameraRotation = 0.0f;
    float mCameraTranslationSpeed = 5.0f;
    float mCameraRotationSpeed = 180.0f;
};
}  // namespace Mantra