#include <MantraEngine.h>

#include <iostream>
#include "imgui.h"

class ExampleLayer : public Mantra::Layer
{
public:
    ExampleLayer() : Layer("Example") {}

    void OnUpdate() override {}

    void OnImGuiRender() override {
        ImGui::Begin("Test");
        ImGui::Text("Hello World");
        ImGui::End();
    }

    void OnEvent(Mantra::Event& event) override {}
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
