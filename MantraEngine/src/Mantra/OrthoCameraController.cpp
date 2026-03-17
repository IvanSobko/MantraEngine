#include "OrthoCameraController.h"

#include "Mantra/Input/Input.h"
#include "Mantra/KeyCodes.h"

namespace Mantra {

OrthoCameraController::OrthoCameraController(float aspectRatio, bool rotation)
    : mAspectRatio(aspectRatio),
      mCamera(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel),
      mRotation(rotation) {}

void OrthoCameraController::OnUpdate(Timestep ts) {
    //TODO: camera position should not be based on rotation
    if (Input::IsKeyPressed(ME_KEY_A)) {
        mCameraPosition.x -= mCameraTranslationSpeed * ts;
    } else if (Input::IsKeyPressed(ME_KEY_D)) {
        mCameraPosition.x += mCameraTranslationSpeed * ts;
    }

    if (Input::IsKeyPressed(ME_KEY_W)) {
        mCameraPosition.y += mCameraTranslationSpeed * ts;
    } else if (Input::IsKeyPressed(ME_KEY_S)) {
        mCameraPosition.y -= mCameraTranslationSpeed * ts;
    }

    if (mRotation) {
        if (Input::IsKeyPressed(ME_KEY_Q)) {
            mCameraRotation += mCameraRotationSpeed * ts;
        } else if (Input::IsKeyPressed(ME_KEY_E)) {
            mCameraRotation -= mCameraRotationSpeed * ts;
        }

        mCamera.SetRotation(mCameraRotation);
    }

    mCamera.SetPosition(mCameraPosition);

    mCameraTranslationSpeed = mZoomLevel;
}

void OrthoCameraController::OnEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseScrolledEvent>(
        std::bind(&OrthoCameraController::OnMouseScrolled, this, std::placeholders::_1));
    dispatcher.Dispatch<WindowResizeEvent>(
        std::bind(&OrthoCameraController::OnWindowResized, this, std::placeholders::_1));
}

bool OrthoCameraController::OnMouseScrolled(MouseScrolledEvent& e) {
    mZoomLevel -= e.GetYOffset() * 0.25f;
    mZoomLevel = std::max(mZoomLevel, 0.25f);
    mCamera.SetProjection(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel);
    return false;
}

bool OrthoCameraController::OnWindowResized(WindowResizeEvent& e) {
    mAspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
    mCamera.SetProjection(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel);
    return false;
}

}  // namespace Mantra