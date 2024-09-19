#include "Application.h"
#include <stdio.h>

#include "Log/Log.h"

#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

namespace Mantra {

Application::Application() {}

Application::~Application() {}

void Application::Run() {
    WindowResizeEvent event(100, 200);

    while (true) {};
}
}  // namespace Mantra