#pragma once

#include <memory>
#include <vector>

namespace Mantra {

enum class FramebufferFormat {
    None = 0,
    RGBA8,  // Color
    RED_INTEGER,
    DEPTH24STENCIL8,  // Depth/stencil
};

// To simplify FrameBuffer constructor arguments, we extract params into these structs.
struct FramebufferAttachment {
    FramebufferAttachment() = default;
    FramebufferAttachment(std::initializer_list<FramebufferFormat> attachments) : attachments(attachments) {}
    std::vector<FramebufferFormat> attachments;
};

struct FramebufferSpecification {
    uint32_t width = 0, height = 0;
    FramebufferAttachment attachments;
};

class Framebuffer
{
public:
    virtual ~Framebuffer() = default;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual void Resize(uint32_t width, uint32_t height) = 0;
    virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;
    virtual const FramebufferSpecification& GetSpecification() const = 0;

    static std::shared_ptr<Framebuffer> Create(const FramebufferSpecification& spec);
};

}  // namespace Mantra