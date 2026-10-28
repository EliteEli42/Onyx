#include "OpenGL/Texture.h"
#include "OpenGL/Image.h"
#include <stb/stb_image.h>
#include <glad/glad.h>

bool Texture::CreateEmpty(
    int width, int height, int nrChannels,
    TextureFilter minFilter, TextureFilter magFilter,
    TextureWrap wrapS, TextureWrap wrapT)
{
    return LoadFromMemory(nullptr, width, height, nrChannels, 1,
        minFilter, magFilter, wrapS, wrapT);
}

bool Texture::LoadFromMemory(
    const unsigned char* data,
    int width,
    int height,
    int nrChannels,
    int layerCount,
    TextureFilter minFilter,
    TextureFilter magFilter,
    TextureWrap wrapS,
    TextureWrap wrapT
)
{
    if (width <= 0 || height <= 0 || nrChannels <= 0 || layerCount <= 0)
        return false;

    Unload();

    // Pixel-Format bestimmen
    GLenum format = GL_RGB;
    switch (nrChannels) {
        case 1: format = GL_RED; break;
        case 2: format = GL_RG; break;
        case 3: format = GL_RGB; break;
        case 4: format = GL_RGBA; break;
        default: return false;
    }

    mWidth = width;
    mHeight = height;
    mNrChannels = nrChannels;
    mLayerCount = layerCount;

    glGenTextures(1, &mID);
    glBindTexture(GL_TEXTURE_2D_ARRAY, mID);

    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, format, width, height, layerCount, 0, format, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, (GLint)minFilter);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, (GLint)magFilter);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, (GLint)wrapS);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, (GLint)wrapT);

    if (minFilter == TextureFilter::LinearMipmapLinear ||
        minFilter == TextureFilter::LinearMipmapNearest ||
        minFilter == TextureFilter::NearestMipmapLinear ||
        minFilter == TextureFilter::NearestMipmapNearest)
    {
        glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    }

    return true;
}

bool Texture::LoadFromFile(
    const std::string& filepath,
    bool flipVertically,
    TextureFilter minFilter,
    TextureFilter magFilter,
    TextureWrap wrapS,
    TextureWrap wrapT
) {
    Image image;
    if (!image.LoadFromFile(filepath, flipVertically))
        return false;

    return LoadFromImage(image, minFilter, magFilter, wrapS, wrapT);
}

bool Texture::LoadFromFiles(
    const std::vector<std::string>& filepaths,
    bool flipVertically,
    TextureFilter minFilter,
    TextureFilter magFilter,
    TextureWrap wrapS,
    TextureWrap wrapT
) {
    Image image;
    if (!image.LoadFromFiles(filepaths, flipVertically))
        return false;

    return LoadFromImage(image, minFilter, magFilter, wrapS, wrapT);
}

bool Texture::LoadFromSpriteSheet(
    const std::string& filepath,
    int spriteWidth,
    int spriteHeight,
    bool flipVertically,
    TextureFilter minFilter,
    TextureFilter magFilter,
    TextureWrap wrapS,
    TextureWrap wrapT
) {
    Image image;
    if (!image.LoadFromSpriteSheet(filepath, spriteWidth, spriteHeight, flipVertically))
        return false;

    return LoadFromImage(image, minFilter, magFilter, wrapS, wrapT);
}

bool Texture::LoadFromSpriteSheets(
    const std::vector<std::string>& filepaths,
    int spriteWidth,
    int spriteHeight,
    bool flipVertically,
    TextureFilter minFilter,
    TextureFilter magFilter,
    TextureWrap wrapS,
    TextureWrap wrapT
) {
    Image image;
    if (!image.LoadFromSpriteSheets(filepaths, spriteWidth, spriteHeight, flipVertically))
        return false;

    return LoadFromImage(image, minFilter, magFilter, wrapS, wrapT);
}

bool Texture::LoadFromImage(
    const Image& image,
    TextureFilter minFilter,
    TextureFilter magFilter,
    TextureWrap wrapS,
    TextureWrap wrapT
)
{
    // Übergibt einfach die Image-Daten an LoadFromMemory
    return LoadFromMemory(
        image.GetData().data(),
        image.GetWidth(),
        image.GetHeight(),
        image.GetNrChannels(),
        image.GetLayerCount(),
        minFilter,
        magFilter,
        wrapS,
        wrapT
    );
}

void Texture::Bind(TextureID unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D_ARRAY, mID);
}

bool Texture::GetData(std::vector<unsigned char>& outData) const
{
    if (mID == 0 || mWidth == 0 || mHeight == 0 || mNrChannels == 0)
        return false;

    // Speicher vorbereiten
    outData.resize(mWidth * mHeight * mNrChannels * mLayerCount);

    // Textur binden
    glBindTexture(GL_TEXTURE_2D_ARRAY, mID);

    // Pixel-Format bestimmen
    GLenum format = GL_RGB;
    switch (mNrChannels) {
        case 1: format = GL_RED; break;
        case 2: format = GL_RG; break;
        case 3: format = GL_RGB; break;
        case 4: format = GL_RGBA; break;
        default: return false; // ungültige Kanalanzahl
    }

    // Pixel von GPU -> CPU
    glGetTexImage(GL_TEXTURE_2D_ARRAY, 0, format, GL_UNSIGNED_BYTE, outData.data());

    return true;
}

void Texture::MoveFrom(Texture&& other) {
	mID = other.mID;
	mWidth = other.mWidth;
	mHeight = other.mHeight;
	mNrChannels = other.mNrChannels;
	mLayerCount = other.mLayerCount;

	other.mID = 0;
	other.mWidth = other.mHeight = other.mNrChannels = other.mLayerCount = 0;
}

void Texture::Unload() {
	if (mID != 0) {
		glDeleteTextures(1, &mID);
		mID = 0;
		mWidth = mHeight = mNrChannels = mLayerCount = 0;
	}
}