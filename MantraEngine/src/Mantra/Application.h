#pragma once

#include "Core.h"
#include "Events/ApplicationEvent.h"
#include "Events/Event.h"
#include "Layers/Layerstack.h"
#include "Window/Window.h"

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

    Layerstack mLayerstack;

    static Application* sInstance;
};

// To be defined in CLIENT
Application* CreateApplication();

}  // namespace Mantra