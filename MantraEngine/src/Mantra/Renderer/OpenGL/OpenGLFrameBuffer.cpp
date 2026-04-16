#include "ME_PCH.h"

#include "OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace Mantra {

static const uint32_t s_MaxFramebufferSize = 8192;

static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width,
                               uint32_t height, int index) {
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, id, 0);
}

static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width,
                               uint32_t height) {
    glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, id, 0);
}

OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec) : mSpecification(spec) {
    for (auto spec : mSpecification.attachments.attachments) {
        if (spec != FramebufferFormat::DEPTH24STENCIL8) {
            mColorAttachmentSpecifications.emplace_back(spec);
        } else {
            mDepthAttachmentSpecification = spec;
        }
    }
    Create();
}

OpenGLFramebuffer::~OpenGLFramebuffer() {
    glDeleteFramebuffers(1, &mRendererID);
    glDeleteTextures(mColorAttachments.size(), mColorAttachments.data());
    glDeleteTextures(1, &mDepthAttachment);
}

void OpenGLFramebuffer::Create() {
    if (mRendererID) {
        glDeleteFramebuffers(1, &mRendererID);
        glDeleteTextures(mColorAttachments.size(), mColorAttachments.data());
        glDeleteTextures(1, &mDepthAttachment);

        mColorAttachments.clear();
        mDepthAttachment = 0;
    }

    glCreateFramebuffers(1, &mRendererID);
    glBindFramebuffer(GL_FRAMEBUFFER, mRendererID);

    // Attachments
    if (mColorAttachmentSpecifications.size()) {
        mColorAttachments.resize(mColorAttachmentSpecifications.size());
        glCreateTextures(GL_TEXTURE_2D, mColorAttachments.size(), mColorAttachments.data());

        for (size_t i = 0; i < mColorAttachments.size(); i++) {
            glBindTexture(GL_TEXTURE_2D, mColorAttachments[i]);
            switch (mColorAttachmentSpecifications[i]) {
                case FramebufferFormat::RGBA8:
                    AttachColorTexture(mColorAttachments[i], 1, GL_RGBA8, GL_RGBA, mSpecification.width,
                                       mSpecification.height, i);
                    break;
                case FramebufferFormat::RED_INTEGER:
                    AttachColorTexture(mColorAttachments[i], 1, GL_R32I, GL_RED_INTEGER, mSpecification.width,
                                       mSpecification.height, i);
                    break;
            }
        }
    }

    if (mDepthAttachmentSpecification != FramebufferFormat::None) {
        glCreateTextures(GL_TEXTURE_2D, 1, &mDepthAttachment);
        glBindTexture(GL_TEXTURE_2D, mDepthAttachment);
        switch (mDepthAttachmentSpecification) {
            case FramebufferFormat::DEPTH24STENCIL8:
                AttachDepthTexture(mDepthAttachment, 1, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT,
                                   mSpecification.width, mSpecification.height);
                break;
        }
    }

    if (mColorAttachments.size() > 1) {
        ME_CORE_ASSERT(mColorAttachments.size() <= 4, "Currently only 4 color attachments are supported!");
        GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
        glDrawBuffers(mColorAttachments.size(), buffers);
    } else if (mColorAttachments.empty()) {
        // Only depth-pass
        glDrawBuffer(GL_NONE);
    }

    ME_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, mRendererID);
    glViewport(0, 0, mSpecification.width, mSpecification.height);
}

void OpenGLFramebuffer::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height) {
    if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize) {
        ME_CORE_WARN("Attempted to rezize framebuffer to {0}, {1}", width, height);
        return;
    }
    mSpecification.width = width;
    mSpecification.height = height;

    Create();
}

uint32_t OpenGLFramebuffer::GetColorAttachmentRendererID(uint32_t index) const {
    ME_CORE_ASSERT(index < mColorAttachments.size(), "Color attachment index out of bounds!");
    return mColorAttachments[index];
}

}  // namespace Mantra