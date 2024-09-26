#pragma once

#include "Input.h"

namespace Mantra {

class GLFWInput : public Input
{
protected:
    bool IsKeyPressedImpl(int keycode) override;
    bool IsMouseButtonPressedImpl(int button) override;
    std::pair<float, float> GetMousePositionImpl() override;
};

}  // namespace Mantra
