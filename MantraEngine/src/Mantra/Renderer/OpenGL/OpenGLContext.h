#pragma once

#include "Mantra/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Mantra {

class OpenGLContext : public GraphicsContext
{

public:
    OpenGLContext(GLFWwindow* windowHandle);

    virtual void Init() override;

    virtual void SwapBuffers() override;

private:
    GLFWwindow* mWindowHandle = nullptr;
};

}  // namespace Mantra