#pragma once

#include "Core.h"
#include "Events/ApplicationEvent.h"
#include "Events/Event.h"
#include "Layers/ImGuiLayer.h"
#include "Layers/Layerstack.h"
#include "Timestep.h"
#include "Window/Window.h"

#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

namespace Mantra {

class MANTRA_API Application
{
public:
    Application();
    virtual ~Application();

    void Run();

    void OnEvent(Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

    inline Window& GetWindow() { return *mWindow; }

    inline static Application& Get() { return *sInstance; }

private:
    bool OnWindowClose(WindowCloseEvent& e);

    std::unique_ptr<Window> mWindow;
    bool mRunning = true;
    float mLastFrameTime = 0.0f;

    Layerstack mLayerstack;
    ImGuiLayer* mImGuiLayer = nullptr;

    static Application* sInstance;
};

// To be defined in CLIENT
Application* CreateApplication();

}  // namespace Mantra