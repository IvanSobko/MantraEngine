#pragma once

#include "Core.h"
#include "Window/Window.h"

namespace Mantra {

class MANTRA_API Application
{
public:
    Application();
    virtual ~Application();

    void Run();

private:
    std::unique_ptr<Window> mWindow;
    bool mRunning = true;
};

// To be defined in CLIENT
Application* CreateApplication();

}  // namespace Mantra