#include "RenderCommand.h"

#include "Mantra/Renderer/OpenGL/OpenGLRendererAPI.h"

namespace Mantra {

//TODO: either make a proper singleton or find a better way to manage this global state
// static pointer + global mutable state = pain in the future
RendererAPI* RenderCommand::sRendererAPI = new OpenGLRendererAPI;

void RenderCommand::Init() {
    sRendererAPI->Init();
}

}  // namespace Mantra