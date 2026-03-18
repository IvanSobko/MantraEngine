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
    glm::vec3 currentCameraPosition = mCamera.GetPosition();
    glm::vec3 currentCameraRotation = mCamera.GetRotation();
    float deltaTime = ts.GetSeconds();

    float yawRad = glm::radians(currentCameraRotation.y);
    float pitchRad = glm::radians(currentCameraRotation.x);
    float rollRad = glm::radians(currentCameraRotation.z);

    // Create rotation matrix that includes all three rotations
    //TODO: expensive, change to quaternions later
    glm::mat3 rotationMatrix = glm::mat3(glm::rotate(glm::mat4(1.0f), yawRad, glm::vec3(0, 1, 0)) *
                                         glm::rotate(glm::mat4(1.0f), pitchRad, glm::vec3(1, 0, 0)) *
                                         glm::rotate(glm::mat4(1.0f), rollRad, glm::vec3(0, 0, 1)));

    // Calculate camera-relative directions using the full rotation matrix
    glm::vec3 forward = -rotationMatrix[2];  // -z in camera space
    glm::vec3 right = rotationMatrix[0];     // +x in camera space
    glm::vec3 up = rotationMatrix[1];        // +y in camera space

    if (Input::IsKeyPressed(ME_KEY_W)) {
        currentCameraPosition += forward * mCameraTranslationSpeed * deltaTime;
    } else if (Input::IsKeyPressed(ME_KEY_S)) {
        currentCameraPosition -= forward * mCameraTranslationSpeed * deltaTime;
    }
    if (Input::IsKeyPressed(ME_KEY_A)) {
        currentCameraPosition -= right * mCameraTranslationSpeed * deltaTime;
    } else if (Input::IsKeyPressed(ME_KEY_D)) {
        currentCameraPosition += right * mCameraTranslationSpeed * deltaTime;
    }

    // Vertical movement (world up/down)
    if (Input::IsKeyPressed(ME_KEY_SPACE)) {
        currentCameraPosition.y += mCameraTranslationSpeed * deltaTime;
    } else if (Input::IsKeyPressed(ME_KEY_LEFT_CONTROL)) {
        currentCameraPosition.y -= mCameraTranslationSpeed * deltaTime;
    }

    auto [x, y] = Input::GetMousePosition();
    const glm::vec2& mouse{x, y};
    glm::vec2 delta = (mouse - mMousePosition) * 0.003f;
    mMousePosition = mouse;

    if (Input::IsMouseButtonPressed(ME_MOUSE_BUTTON_RIGHT)) {
        currentCameraRotation.y += delta.x * mCameraRotationSpeed;
        currentCameraRotation.x += delta.y * mCameraRotationSpeed;
    }

    if (Input::IsKeyPressed(ME_KEY_Q)) {
        currentCameraRotation.z -= mCameraRotationSpeed * deltaTime;
    } else if (Input::IsKeyPressed(ME_KEY_E)) {
        currentCameraRotation.z += mCameraRotationSpeed * deltaTime;
    }

    // Clamp pitch to avoid gimbal lock
    currentCameraRotation.x = std::clamp(currentCameraRotation.x, -89.0f, 89.0f);

    mCamera.SetPosition(currentCameraPosition);
    mCamera.SetRotation(currentCameraRotation);

    float fov = 0.0f;
    if constexpr (std::is_same_v<CameraType, PerspectiveCamera>) {
        fov = mCamera.GetFOV();
    }

    MLOG("Camera position: ({:.2f}, {:.2f}, {:.2f}), rotation: ({:.2f}, {:.2f}, {:.2f}), fov: {:.2f}",
         currentCameraPosition.x, currentCameraPosition.y, currentCameraPosition.z, currentCameraRotation.x,
         currentCameraRotation.y, currentCameraRotation.z, fov);
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