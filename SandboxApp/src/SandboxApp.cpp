#include <MantraEngine.h>

#include <iostream>

class Sandbox : public Mantra::Application
{
public:
    Sandbox() {}

    ~Sandbox() {}
};

Mantra::Application* Mantra::CreateApplication() {
    return new Sandbox();
}
