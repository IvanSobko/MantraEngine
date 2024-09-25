#include <MantraEngine.h>

#include <iostream>

class ExampleLayer : public Mantra::Layer
{
public:
    ExampleLayer() : Layer("Example") {}

    void OnUpdate() override { ME_INFO("ExampleLayer::Update"); }

    void OnEvent(Mantra::Event& event) override { ME_TRACE("{0}", event); }
};

class Sandbox : public Mantra::Application
{
public:
    Sandbox() { PushLayer(new ExampleLayer()); }

    ~Sandbox() {}
};

Mantra::Application* Mantra::CreateApplication() {
    return new Sandbox();
}
