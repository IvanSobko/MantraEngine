#include "WindowsWindow.h"
#include "ME_PCH.h"

namespace Mantra {

static bool GLFWInitialized = false;

Window* Window::Create(const WindowProps& props) {
    return new WindowsWindow(props);
}

WindowsWindow::WindowsWindow(const WindowProps& props) {
    Init(props);
}

WindowsWindow::~WindowsWindow() {
    Shutdown();
}

void WindowsWindow::Init(const WindowProps& props) {
    mData.title = props.title;
    mData.width = props.width;
    mData.height = props.height;

    ME_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width, props.height);

    if (!GLFWInitialized) {
        // TODO: glfwTerminate on system shutdown
        int success = glfwInit();
        if (!success) {
            ME_CORE_ERROR("Could not intialize GLFW!");
        }

        GLFWInitialized = true;
    }

    mWindow = glfwCreateWindow((int)props.width, (int)props.height, mData.title.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(mWindow);
    glfwSetWindowUserPointer(mWindow, &mData);
    SetVSync(true);
}

void WindowsWindow::Shutdown() {
    glfwDestroyWindow(mWindow);
}

void WindowsWindow::OnUpdate() {
    glfwPollEvents();
    glfwSwapBuffers(mWindow);
}

void WindowsWindow::SetVSync(bool enabled) {
    if (enabled)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);

    mData.vSync = enabled;
}

bool WindowsWindow::IsVSync() const {
    return mData.vSync;
}

}  // namespace Mantra
