#pragma once

#include "Mantra/Core.h"

namespace Mantra {

class MANTRA_API Input
{
public:
    inline static bool IsKeyPressed(int keycode) { return sInstance->IsKeyPressedImpl(keycode); }
    inline static bool IsMouseButtonPressed(int button) { return sInstance->IsMouseButtonPressedImpl(button); }
    inline static std::pair<float, float> GetMousePosition() { return sInstance->GetMousePositionImpl(); }

protected:
    virtual bool IsKeyPressedImpl(int keycode) = 0;
    virtual bool IsMouseButtonPressedImpl(int button) = 0;
    virtual std::pair<float, float> GetMousePositionImpl() = 0;

private:
    static Input* sInstance;
};

}  // namespace Mantra