#include "GLFWWindow.h"

#include "Mantra/Events/ApplicationEvent.h"
#include "Mantra/Events/KeyEvent.h"
#include "Mantra/Events/MouseEvent.h"

#include "Mantra/Renderer/OpenGL/OpenGLContext.h"

namespace Mantra {

static bool GLFWInitialized = false;

Window* Window::Create(const WindowProps& props) {
    return new GLFWWindow(props);
}

GLFWWindow::GLFWWindow(const WindowProps& props) {
    Init(props);
}

GLFWWindow::~GLFWWindow() {
    Shutdown();
}

void GLFWWindow::Init(const WindowProps& props) {
    mData.title = props.title;
    mData.width = props.width;
    mData.height = props.height;

    ME_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width, props.height);

    if (!GLFWInitialized) {
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

    mContext = new OpenGLContext(mWindow);
    mContext->Init();

    glfwSetWindowUserPointer(mWindow, &mData);
    SetVSync(true);

    glfwSetKeyCallback(mWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        switch (action) {
            case GLFW_PRESS: {
                KeyPressedEvent event(key);
                data.eventCallback(event);
                break;
            }
            case GLFW_RELEASE: {
                KeyReleasedEvent event(key);
                data.eventCallback(event);
                break;
            }
            case GLFW_REPEAT: {
                KeyPressedEvent event(key, 1);
                data.eventCallback(event);
                break;
            }
        }
    });

    glfwSetCharCallback(mWindow, [](GLFWwindow* window, unsigned int keycode) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        KeyTypedEvent event(keycode);
        data.eventCallback(event);
    });

    glfwSetMouseButtonCallback(mWindow, [](GLFWwindow* window, int button, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        switch (action) {
            case GLFW_PRESS: {
                MouseButtonPressedEvent event(button);
                data.eventCallback(event);
                break;
            }
            case GLFW_RELEASE: {
                MouseButtonReleasedEvent event(button);
                data.eventCallback(event);
                break;
            }
        }
    });

    glfwSetCursorPosCallback(mWindow, [](GLFWwindow* window, double xpos, double ypos) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseMovedEvent event((float)xpos, (float)ypos);
        data.eventCallback(event);
    });

    glfwSetScrollCallback(mWindow, [](GLFWwindow* window, double xoffset, double yoffset) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseScrolledEvent event((float)xoffset, (float)yoffset);
        data.eventCallback(event);
    });

    glfwSetWindowCloseCallback(mWindow, [](GLFWwindow* window) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        WindowCloseEvent event;
        data.eventCallback(event);
    });

    glfwSetWindowSizeCallback(mWindow, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.width = width;
        data.height = height;
        WindowResizeEvent event(width, height);
        data.eventCallback(event);
    });
}

void GLFWWindow::Shutdown() {
    glfwDestroyWindow(mWindow);

    if (GLFWInitialized) {
        ME_CORE_INFO("GLFW terminated.");
        glfwTerminate();
    }
}

void GLFWWindow::OnUpdate() {
    glfwPollEvents();
    mContext->SwapBuffers();
}

void* GLFWWindow::GetNativeWindow() const {
    return mWindow;
}

void GLFWWindow::SetVSync(bool enabled) {
    if (enabled) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }

    mData.vSync = enabled;
}

bool GLFWWindow::IsVSync() const {
    return mData.vSync;
}

}  // namespace Mantra
