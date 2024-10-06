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

    void OnAttach() override;
    void OnDetach() override;
    void OnImGuiRender() override;

    void Begin();
    void End();
};

}  // namespace Mantra