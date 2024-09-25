#pragma once

#include "Layer.h"
#include "Mantra/Core.h"

#include <vector>

namespace Mantra {

class MANTRA_API Layerstack
{
public:
    Layerstack();
    ~Layerstack();

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);
    void PopLayer(Layer* layer);
    void PopOverlay(Layer* overlay);

    std::vector<Layer*>::iterator begin() { return mLayers.begin(); }
    std::vector<Layer*>::iterator end() { return mLayers.end(); }

private:
    std::vector<Layer*> mLayers;
    std::vector<Layer*>::iterator mLayerInsert;
};

}  // namespace Mantra