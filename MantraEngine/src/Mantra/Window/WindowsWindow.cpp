#include "WindowsWindow.h"
#include "ME_PCH.h"

#include "Mantra/Events/ApplicationEvent.h"
#include "Mantra/Events/KeyEvent.h"
#include "Mantra/Events/MouseEvent.h"

#include <glad/glad.h>

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

    glfwSetErrorCallback(
        [](int error, const char* description) { ME_CORE_ERROR("GLFW error: {0}:{1}", error, description); });

    mWindow = glfwCreateWindow((int)props.width, (int)props.height, mData.title.c_str(), nullptr, nullptr);
    if (!mWindow) {
        ME_CORE_ERROR("Failed to create OpenGL context.");
        return;
    }

    glfwMakeContextCurrent(mWindow);

    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    ME_CORE_ASSERT(status, "Failed to initialize Glad!");

    glfwSetWindowUserPointer(mWindow, &mData);
    SetVSync(true);

    glfwSetKeyCallback(mWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        switch (action) {
            case GLFW_PRESS:
                data.eventCallback(KeyPressedEvent(key, 0));
                break;
            case GLFW_RELEASE:
                data.eventCallback(KeyReleasedEvent(key));
                break;
            case GLFW_REPEAT:
                data.eventCallback(KeyPressedEvent(key, 1));
                break;
        }
    });

    glfwSetMouseButtonCallback(mWindow, [](GLFWwindow* window, int button, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        switch (action) {
            case GLFW_PRESS:
                data.eventCallback(MouseButtonPressedEvent(button));
                break;
            case GLFW_RELEASE:
                data.eventCallback(MouseButtonReleasedEvent(button));
                break;
        }
    });

    glfwSetCursorPosCallback(mWindow, [](GLFWwindow* window, double xpos, double ypos) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        data.eventCallback(MouseMovedEvent((float)xpos, (float)ypos));
    });

    glfwSetScrollCallback(mWindow, [](GLFWwindow* window, double xoffset, double yoffset) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        data.eventCallback(MouseScrolledEvent((float)xoffset, (float)yoffset));
    });

    glfwSetWindowCloseCallback(mWindow, [](GLFWwindow* window) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.eventCallback(WindowCloseEvent());
    });

    glfwSetWindowSizeCallback(mWindow, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.width = width;
        data.height = height;

        data.eventCallback(WindowResizeEvent(width, height));
    });
}

void WindowsWindow::Shutdown() {
    glfwDestroyWindow(mWindow);
}

void WindowsWindow::OnUpdate() {
    glfwPollEvents();
    glfwSwapBuffers(mWindow);
}

void WindowsWindow::SetVSync(bool enabled) {
    if (enabled) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }

    mData.vSync = enabled;
}

bool WindowsWindow::IsVSync() const {
    return mData.vSync;
}

}  // namespace Mantra
