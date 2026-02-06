#include "Application.h"

#include <GLFW/glfw3.h>

#include "Mantra/Renderer/Renderer.h"

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

    mImGuiLayer = new ImGuiLayer();
    PushOverlay(mImGuiLayer);
}

Application::~Application() {
    // glfwTerminate();
}

void Application::Run() {
    while (mRunning) {
        float time = (float)glfwGetTime();
        Timestep timestep = time - mLastFrameTime;
        mLastFrameTime = time;

        for (Layer* layer : mLayerstack) {
            layer->OnUpdate(timestep);
        }

        mImGuiLayer->Begin();
        for (Layer* layer : mLayerstack) {
            layer->OnImGuiRender();
        }
        mImGuiLayer->End();

        mWindow->OnUpdate();
    }
}

void Application::OnEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));

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