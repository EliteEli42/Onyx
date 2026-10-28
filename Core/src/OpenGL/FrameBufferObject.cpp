#include <OpenGL/FrameBufferObject.h>
#include <glad/glad.h>

bool FrameBufferObject::Create(uint32_t width, uint32_t height, uint32_t nrChannels) {
    Destroy();
    mWidth = width;
    mHeight = height;
    mNrChannels = nrChannels;

    if (!mColorTex.CreateEmpty(width, height, nrChannels))
        return false;

    glGenRenderbuffers(1, &mDepthRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, mDepthRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glGenFramebuffers(1, &mFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
    glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mColorTex.GetID(), 0, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mDepthRBO);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    sBoundRead = sBoundWrite = 0;
    return IsComplete();
}

FrameBufferObject::~FrameBufferObject() {
    Destroy();
}

void FrameBufferObject::Destroy() {
    if (mFBO) { glDeleteFramebuffers(1, &mFBO);       mFBO = 0; }
    if (mDepthRBO) { glDeleteRenderbuffers(1, &mDepthRBO); mDepthRBO = 0; }
    mColorTex.Unload();
    mWidth = mHeight = 0;
}

bool FrameBufferObject::Resize(uint32_t width, uint32_t height) {
    if (width == mWidth && height == mHeight) return true;
    if (mFBO == 0) return false;

    mWidth = width;
    mHeight = height;

    if (!mColorTex.CreateEmpty(width, height, mNrChannels))
        return false;

    // FBO neu verknüpfen da Texture-ID sich geändert hat
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
    glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mColorTex.GetID(), 0, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, mDepthRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    return IsComplete();
}

bool FrameBufferObject::IsComplete() const {
    Bind();
    bool complete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    Unbind();
    return complete;
}

void FrameBufferObject::Bind(FBOTarget target) const {
    GLenum t = static_cast<GLenum>(target);
    if (target == FBOTarget::Read) {
        if (sBoundRead != mFBO) { glBindFramebuffer(t, mFBO); sBoundRead = mFBO; }
    }
    else if (target == FBOTarget::Write) {
        if (sBoundWrite != mFBO) { glBindFramebuffer(t, mFBO); sBoundWrite = mFBO; }
    }
    else {
        if (sBoundRead != mFBO || sBoundWrite != mFBO) {
            glBindFramebuffer(t, mFBO);
            sBoundRead = sBoundWrite = mFBO;
        }
    }
}

void FrameBufferObject::Unbind(FBOTarget target) const {
    GLenum t = static_cast<GLenum>(target);
    if (target == FBOTarget::Read) {
        if (sBoundRead == mFBO) { glBindFramebuffer(t, 0); sBoundRead = 0; }
    }
    else if (target == FBOTarget::Write) {
        if (sBoundWrite == mFBO) { glBindFramebuffer(t, 0); sBoundWrite = 0; }
    }
    else {
        if (sBoundRead == mFBO || sBoundWrite == mFBO) {
            glBindFramebuffer(t, 0);
            sBoundRead = sBoundWrite = 0;
        }
    }
}