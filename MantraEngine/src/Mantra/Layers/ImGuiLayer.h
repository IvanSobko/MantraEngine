#pragma once

#include "Layer.h"

#include "Mantra/Events/ApplicationEvent.h"
#include "Mantra/Events/KeyEvent.h"
#include "Mantra/Events/MouseEvent.h"

namespace Mantra {

class MANTRA_API ImGuiLayer : public Layer
{
public:
    ImGuiLayer();
    ~ImGuiLayer();

    void OnAttach();
    void OnDetach();
    void OnUpdate();
    void OnEvent(Event& event);

private:
    float mTime = 0.0f;
};

}  // namespace Mantra