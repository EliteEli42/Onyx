#pragma once
#include <Common/MathTypes.h>
#include <OpenGL/Texture.h>

enum class FBOTarget : uint32_t {
    ReadWrite = 0x8D40,
    Read = 0x8CA8,
    Write = 0x8CA9
};

class FrameBufferObject {
public:
    bool Create(uint32_t width, uint32_t height, uint32_t nrChannels = 4);
    ~FrameBufferObject();
    void Destroy();
    bool Resize(uint32_t width, uint32_t height);
    void Bind(FBOTarget target = FBOTarget::ReadWrite) const;
    void Unbind(FBOTarget target = FBOTarget::ReadWrite) const;
    bool IsComplete() const;
    bool IsValid() const { return mFBO != 0; }

    const Texture& GetTexture() const { return mColorTex; }
    uint32_t GetWidth()  const { return mWidth; }
    uint32_t GetHeight() const { return mHeight; }

private:
    uint32_t mFBO = 0;
    uint32_t mDepthRBO = 0;
    uint32_t mWidth = 0;
    uint32_t mHeight = 0;
    uint32_t mNrChannels = 0;
    Texture  mColorTex;

    static inline uint32_t sBoundRead = 0;
    static inline uint32_t sBoundWrite = 0;
};