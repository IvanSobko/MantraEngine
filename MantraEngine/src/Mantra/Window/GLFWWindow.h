#pragma once

#include "Window.h"

#include <GLFW/glfw3.h>

namespace Mantra {

class GLFWWindow : public Window
{
public:
    GLFWWindow(const WindowProps& props);
    virtual ~GLFWWindow();

    void OnUpdate() override;

    inline unsigned int GetWidth() const override { return mData.width; }
    inline unsigned int GetHeight() const override { return mData.height; }

    void* GetNativeWindow() const override;

    // Window attributes
    inline void SetEventCallback(const EventCallbackFn& callback) override { mData.eventCallback = callback; }
    void SetVSync(bool enabled) override;
    bool IsVSync() const override;

private:
    virtual void Init(const WindowProps& props);
    virtual void Shutdown();

    GLFWwindow* mWindow = nullptr;

    struct WindowData {
        std::string title;
        unsigned int width, height;
        bool vSync;

        EventCallbackFn eventCallback;
    };

    WindowData mData;
};

}  // namespace Mantra