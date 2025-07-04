#pragma once
#include <iostream>

#include <MantraEngine.h>

extern Mantra::Application* Mantra::CreateApplication();

int main(int argc, char** argv) {
    Mantra::Log::Init();

    auto app = Mantra::CreateApplication();
    app->Run();
    delete app;
}
