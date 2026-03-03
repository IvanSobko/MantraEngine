#include "RenderCommand.h"

#include "Mantra/Renderer/OpenGL/OpenGLRendererAPI.h"

namespace Mantra {

RendererAPI* RenderCommand::sRendererAPI = new OpenGLRendererAPI;

void RenderCommand::Init() {
    sRendererAPI->Init();
}

}  // namespace Mantra