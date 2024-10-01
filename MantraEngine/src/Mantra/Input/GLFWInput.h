#pragma once

#include "Input.h"

namespace Mantra {

//TODO: add own key code and conversion between plaforms Win32, GLFW, etc..

class GLFWInput : public Input
{
protected:
    bool IsKeyPressedImpl(int keycode) override;
    bool IsMouseButtonPressedImpl(int button) override;
    std::pair<float, float> GetMousePositionImpl() override;
};

}  // namespace Mantra
