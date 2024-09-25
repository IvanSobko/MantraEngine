#include "Layerstack.h"

namespace Mantra {

Layerstack::Layerstack() {
    mLayerInsert = mLayers.begin();
}

Layerstack::~Layerstack() {
    for (Layer* layer : mLayers) {
        delete layer;
    }
}

void Layerstack::PushLayer(Layer* layer) {
    mLayerInsert = mLayers.emplace(mLayerInsert, layer);
}

void Layerstack::PushOverlay(Layer* overlay) {
    mLayers.emplace_back(overlay);
}

void Layerstack::PopLayer(Layer* layer) {
    auto it = std::find(mLayers.begin(), mLayers.end(), layer);
    if (it != mLayers.end()) {
        mLayers.erase(it);
        mLayerInsert--;
    }
}

void Layerstack::PopOverlay(Layer* overlay) {
    auto it = std::find(mLayers.begin(), mLayers.end(), overlay);
    if (it != mLayers.end()) {
        mLayers.erase(it);
    }
}

}  // namespace Mantra