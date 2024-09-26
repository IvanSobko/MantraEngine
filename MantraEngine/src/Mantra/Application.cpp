#include "Application.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include "ME_PCH.h"

namespace Mantra {
Application* Application::sInstance = nullptr;

Application::Application() {
    if (sInstance) {
        ME_CORE_ERROR("Trying to create another Application instance.");
        assert(true);
        return;
    }

    sInstance = this;

    mWindow = std::unique_ptr<Window>(Window::Create());
    mWindow->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
}

Application::~Application() {
    // glfwTerminate();
}

void Application::Run() {
    while (mRunning) {
        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (Layer* layer : mLayerstack) {
            layer->OnUpdate();
        }
        mWindow->OnUpdate();
    }
}
void Application::OnEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(
        std::bind(&Application::OnWindowClose, this, std::placeholders::_1));

    for (auto it = mLayerstack.end(); it != mLayerstack.begin();) {
        (*--it)->OnEvent(e);
        if (e.handled) {
            break;
        }
    }
}

void Application::PushLayer(Layer* layer) {
    mLayerstack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverlay(Layer* layer) {
    mLayerstack.PushOverlay(layer);
    layer->OnAttach();
}

bool Application::OnWindowClose(WindowCloseEvent& e) {
    mRunning = false;
    //TODO: cleanup glfw context
    return true;
}
}  // namespace Mantra