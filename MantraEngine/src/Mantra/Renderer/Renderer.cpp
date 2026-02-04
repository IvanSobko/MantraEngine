#include "Renderer.h"

namespace Mantra {

// for now we hardcode OpenGL as the only rendering API
RendererAPI Renderer::sRendererAPI = RendererAPI::OpenGL;

}  // namespace Mantra