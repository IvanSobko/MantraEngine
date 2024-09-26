#include "GLFWInput.h"

#include "ME_PCH.h"

#include <GLFW/glfw3.h>

#include "Mantra/Application.h"

namespace Mantra {

Input* Mantra::Input::sInstance = new GLFWInput();

bool GLFWInput::IsKeyPressedImpl(int keycode) {
    GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    int state = glfwGetKey(window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool GLFWInput::IsMouseButtonPressedImpl(int button) {
    GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    int state = glfwGetMouseButton(window, button);
    return state == GLFW_PRESS;
}

std::pair<float, float> GLFWInput::GetMousePositionImpl() {
    GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    return {(float)x, (float)y};
}

}  // namespace Mantra