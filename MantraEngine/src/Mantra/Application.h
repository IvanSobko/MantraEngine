#pragma once

#include "Core.h"

namespace Mantra {

class MANTRA_API Application
{
public:
    Application();
    virtual ~Application();

    void Run();
};

// To be defined in CLIENT
Application* CreateApplication();

}  // namespace Mantra