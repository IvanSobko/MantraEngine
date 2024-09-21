#include "Application.h"
#include <stdio.h>

#include "ME_PCH.h"

#include <GLFW/glfw3.h>

namespace Mantra {

Application::Application() {
    mWindow = std::unique_ptr<Window>(Window::Create());
}

Application::~Application() {}

void Application::Run() {
    while (mRunning) {
        glClearColor(1, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        mWindow->OnUpdate();
    }
}
}  // namespace Mantra