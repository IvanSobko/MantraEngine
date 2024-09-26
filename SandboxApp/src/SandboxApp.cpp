#include <MantraEngine.h>

#include <iostream>

class ExampleLayer : public Mantra::Layer
{
public:
    ExampleLayer() : Layer("Example") {}

    void OnUpdate() override {}

    void OnEvent(Mantra::Event& event) override {}
};

class Sandbox : public Mantra::Application
{
public:
    Sandbox() {
        PushLayer(new ExampleLayer());
        PushOverlay(new Mantra::ImGuiLayer());
    }

    ~Sandbox() {}
};

Mantra::Application* Mantra::CreateApplication() {
    return new Sandbox();
}
