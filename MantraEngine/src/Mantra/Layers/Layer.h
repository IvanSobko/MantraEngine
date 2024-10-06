#pragma once

#include "Mantra/Core.h"
#include "Mantra/Events/Event.h"

namespace Mantra {

class MANTRA_API Layer
{
public:
    Layer(const std::string& name = "Layer") : mDebugName(name) {};
    ~Layer() {};

    virtual void OnAttach() {};
    virtual void OnDetach() {};
    virtual void OnUpdate() {};
    virtual void OnEvent(Event& event) {};
    virtual void OnImGuiRender() {};

    inline const std::string& GetName() const { return mDebugName; }

protected:
    std::string mDebugName;
};

}  // namespace Mantra