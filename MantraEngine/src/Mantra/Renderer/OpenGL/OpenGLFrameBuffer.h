#pragma once

#include "Mantra/Renderer/Framebuffer.h"

#include "Mantra/Core.h"

namespace Mantra {

class OpenGLFramebuffer : public Framebuffer
{
public:
    OpenGLFramebuffer(const FramebufferSpecification& spec);
    virtual ~OpenGLFramebuffer() override;

    virtual void Bind() override;
    virtual void Unbind() override;

    virtual void Resize(uint32_t width, uint32_t height) override;

    virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override;

    virtual const FramebufferSpecification& GetSpecification() const override { return mSpecification; }

private:
    void Create();

    uint32_t mRendererID = 0;
    FramebufferSpecification mSpecification;

    std::vector<FramebufferFormat> mColorAttachmentSpecifications;
    FramebufferFormat mDepthAttachmentSpecification = FramebufferFormat::None;

    std::vector<uint32_t> mColorAttachments;
    uint32_t mDepthAttachment = 0;
};

}  // namespace Mantra