#pragma once

#include "Core.h"
#include "Events/ApplicationEvent.h"
#include "Events/Event.h"
#include "Window/Window.h"

namespace Mantra {

class MANTRA_API Application
{
public:
    Application();
    virtual ~Application();

    void Run();

    void OnEvent(Event& e);

private:
    bool OnWindowClose(WindowCloseEvent& e);

    std::unique_ptr<Window> mWindow;
    bool mRunning = true;
};

// To be defined in CLIENT
Application* CreateApplication();

}  // namespace Mantra