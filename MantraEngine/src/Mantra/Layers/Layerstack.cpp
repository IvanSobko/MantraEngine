#include "Layerstack.h"

namespace Mantra {

Layerstack::Layerstack() {}

Layerstack::~Layerstack() {
    for (Layer* layer : mLayers) {
        delete layer;
    }
}

void Layerstack::PushLayer(Layer* layer) {
    mLayers.emplace(mLayers.begin() + mLayerInsertIndex, layer);
    mLayerInsertIndex++;
}

void Layerstack::PushOverlay(Layer* overlay) {
    mLayers.emplace_back(overlay);
}

void Layerstack::PopLayer(Layer* layer) {
    auto it = std::find(mLayers.begin(), mLayers.end(), layer);
    if (it != mLayers.end()) {
        mLayers.erase(it);
        mLayerInsertIndex--;
    }
}

void Layerstack::PopOverlay(Layer* overlay) {
    auto it = std::find(mLayers.begin(), mLayers.end(), overlay);
    if (it != mLayers.end()) {
        mLayers.erase(it);
    }
}

}  // namespace Mantra