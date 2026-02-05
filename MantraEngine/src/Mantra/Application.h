#pragma once

#include "Core.h"
#include "Events/ApplicationEvent.h"
#include "Events/Event.h"
#include "Layers/ImGuiLayer.h"
#include "Layers/Layerstack.h"
#include "Window/Window.h"

#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

namespace Mantra {

class MANTRA_API Application
{
public:
    Application();
    virtual ~Application();

    void Run();

    void OnEvent(Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

    inline Window& GetWindow() { return *mWindow; }

    inline static Application& Get() { return *sInstance; }

private:
    bool OnWindowClose(WindowCloseEvent& e);

    std::unique_ptr<Window> mWindow;
    bool mRunning = true;

    Layerstack mLayerstack;
    ImGuiLayer* mImGuiLayer = nullptr;

    std::shared_ptr<Shader> mTriangleShader;
    std::shared_ptr<VertexArray> mTriangleVA;

    std::shared_ptr<Shader> mSquareShader;
    std::shared_ptr<VertexArray> mSquareVA;

    static Application* sInstance;
};

// To be defined in CLIENT
Application* CreateApplication();

}  // namespace Mantra