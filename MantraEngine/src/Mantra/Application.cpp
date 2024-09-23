#include "Application.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include "ME_PCH.h"

namespace Mantra {

Application::Application() {
    mWindow = std::unique_ptr<Window>(Window::Create());
    mWindow->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
}

Application::~Application() {
    // glfwTerminate();
}

void Application::Run() {
    while (mRunning) {
        glClearColor(1, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        mWindow->OnUpdate();
    }
}
void Application::OnEvent(Event& e) {
    ME_CORE_TRACE("{0}", e);

    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(
        std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
}
bool Application::OnWindowClose(WindowCloseEvent& e) {
    mRunning = false;
    //TODO: cleanup glfw context
    return true;
}
}  // namespace Mantra