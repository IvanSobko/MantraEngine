#pragma once

#include "Mantra/Core.h"

#include <glad/glad.h>

namespace Mantra {
class OpenGLDebug
{
public:
    static void Enable() {
#ifdef ME_DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(MessageCallback, nullptr);

        // Filter out low-severity messages
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif
    }

private:
    static void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                           const GLchar* message, const void* userParam) {
        const char* severityStr = (severity == GL_DEBUG_SEVERITY_HIGH)     ? "HIGH"
                                  : (severity == GL_DEBUG_SEVERITY_MEDIUM) ? "MEDIUM"
                                  : (severity == GL_DEBUG_SEVERITY_LOW)    ? "LOW"
                                                                           : "NOTIFICATION";

        ME_CORE_ERROR("OpenGL [{0}]: {1}", severityStr, message);

        if (severity == GL_DEBUG_SEVERITY_HIGH) {
            ME_CORE_ASSERT(false, "Critical OpenGL Error!");
        }
    }
};
}  // namespace Mantra