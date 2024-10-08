#pragma once
#include <iostream>

#include <MantraEngine.h>
#ifdef ME_PLATFORM_WINDOWS

extern Mantra::Application* Mantra::CreateApplication();

int main(int argc, char** argv) {
    Mantra::Log::Init();

    auto app = Mantra::CreateApplication();
    app->Run();
    delete app;
}

#endif