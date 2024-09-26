#pragma once

#include "Window.h"

#include <GLFW/glfw3.h>

namespace Mantra {

class WindowsWindow : public Window
{
public:
    WindowsWindow(const WindowProps& props);
    virtual ~WindowsWindow();

    void OnUpdate() override;

    inline unsigned int GetWidth() const override { return mData.width; }
    inline unsigned int GetHeight() const override { return mData.height; }

    bool IsKeyDown(int keycode) const override;

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