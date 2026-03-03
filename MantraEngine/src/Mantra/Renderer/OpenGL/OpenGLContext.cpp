#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "OpenGLDebug.h"

namespace Mantra {

OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : mWindowHandle(windowHandle) {

    ME_CORE_ASSERT(windowHandle, "Window handle is null!")
}

void OpenGLContext::Init() {

    glfwMakeContextCurrent(mWindowHandle);

    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    ME_CORE_ASSERT(status, "Failed to initialize Glad!");

    ME_CORE_INFO("OpenGL Info:");
    ME_CORE_INFO("  Vendor  : {0}", (const char*)glGetString(GL_VENDOR));
    ME_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
    ME_CORE_INFO("  Version : {0}", (const char*)glGetString(GL_VERSION));

    OpenGLDebug::Enable();
}

void OpenGLContext::SwapBuffers() {

    glfwSwapBuffers(mWindowHandle);
}

}  // namespace Mantra