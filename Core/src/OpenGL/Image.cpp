#include <Core/OpenGL/Image.h>
#include <Core/OpenGL/Texture.h>

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

using namespace Core;

void Image::CreateSolidColor(int width, int height, int nrChannels, int layerCount, Color color)
{
    if (width <= 0 || height <= 0 || nrChannels <= 0 || layerCount <= 0)
        return;

    mWidth = width;
    mHeight = height;
    mNrChannels = nrChannels;
    mLayerCount = layerCount;

    size_t pixelCount = (size_t)width * height;
    size_t totalSize = pixelCount * nrChannels * layerCount;

    mData.resize(totalSize);

    for (int i = 0; i < mLayerCount; i++)
    {
        FillLayer(i, color);
    }
}

void Image::AddSolidColorLayer(Color color)
{
    if (mWidth <= 0 || mHeight <= 0 || mNrChannels <= 0)
        return;

    size_t pixelCount = (size_t)mWidth * mHeight;
    size_t layerSize = pixelCount * mNrChannels;

    size_t newLayerIndex = mLayerCount;
    mLayerCount++;

    mData.resize(layerSize * mLayerCount);

	FillLayer(static_cast<int>(newLayerIndex), color);
}

void Image::CreateEmpty(int width, int height, int nrChannels)
{
	mWidth = width;
	mHeight = height;
	mNrChannels = nrChannels;
	mLayerCount = 0;

    mData.resize(width * height * nrChannels);
}

bool Image::LoadFromMemory(const unsigned char* data, int width, int height, int nrChannels, int layerCount)
{
    if (!data || width <= 0 || height <= 0 || nrChannels <= 0 || layerCount <= 0)
        return false;

    mWidth = width;
    mHeight = height;
    mNrChannels = nrChannels;
    mLayerCount = layerCount;

    size_t totalSize = static_cast<size_t>(width * height * nrChannels * layerCount);

    mData.resize(totalSize);
    std::memcpy(mData.data(), data, totalSize);

    return true;
}

bool Image::LoadFromFile(const std::string& filepath, bool flipVertically)
{
    stbi_set_flip_vertically_on_load(flipVertically ? 1 : 0);

    unsigned char* data = stbi_load(filepath.c_str(), &mWidth, &mHeight, &mNrChannels, 0);
    if (!data)
        return false;

    mLayerCount = 1;

    size_t totalSize = static_cast<size_t>(mWidth) * mHeight * mNrChannels;
    mData.resize(totalSize);
    std::memcpy(mData.data(), data, totalSize);

    stbi_image_free(data);

    return true;
}

bool Image::LoadFromFiles(const std::vector<std::string>& filepaths, bool flipVertically)
{
    if (filepaths.empty())
        return false;

    stbi_set_flip_vertically_on_load(flipVertically ? 1 : 0);

    unsigned char* first = stbi_load(filepaths[0].c_str(), &mWidth, &mHeight, &mNrChannels, 0);
    if (!first)
        return false;

    mLayerCount = static_cast<int>(filepaths.size());

    size_t layerSize = static_cast<size_t>(mWidth) * mHeight * mNrChannels;
    size_t totalSize = layerSize * mLayerCount;

    mData.resize(totalSize);

    std::memcpy(mData.data(), first, layerSize);
    stbi_image_free(first);

    for (size_t i = 1; i < filepaths.size(); i++)
    {
        int w, h, c;
        unsigned char* data = stbi_load(filepaths[i].c_str(), &w, &h, &c, 0);

        if (!data)
        {
            mData.clear();
            mLayerCount = 0;
            return false;
        }

        if (w != mWidth || h != mHeight || c != mNrChannels)
        {
            stbi_image_free(data);
            return false;
        }

        std::memcpy(mData.data() + i * layerSize, data, layerSize);

        stbi_image_free(data);
    }

    return true;
}

bool Image::AddLayerFromFile(const std::string& filepath, bool flipVertically)
{
    stbi_set_flip_vertically_on_load(flipVertically ? 1 : 0);

    int width, height, channels;

    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
    if (!data)
        return false;

    if (mLayerCount > 0 &&
        (width != mWidth || height != mHeight || channels != mNrChannels))
    {
        stbi_image_free(data);
        return false;
    }

    if (mLayerCount == 0)
    {
        mWidth = width;
        mHeight = height;
        mNrChannels = channels;
    }

    size_t layerSize = static_cast<size_t>(mWidth) * mHeight * mNrChannels;

    size_t newLayerIndex = mLayerCount;

    mLayerCount++;

    mData.resize(layerSize * mLayerCount);

    std::memcpy(mData.data() + newLayerIndex * layerSize, data, layerSize);

    stbi_image_free(data);

    return true;
}

bool Image::AddLayersFromFiles(const std::vector<std::string>& filepaths, bool flipVertically)
{
    if (filepaths.empty())
        return false;

    stbi_set_flip_vertically_on_load(flipVertically ? 1 : 0);

    size_t newLayers = filepaths.size();

    int width = 0, height = 0, channels = 0;

    if (mLayerCount == 0)
    {
        unsigned char* first = stbi_load(filepaths[0].c_str(), &width, &height, &channels, 0);
        if (!first)
            return false;

        mWidth = width;
        mHeight = height;
        mNrChannels = channels;

        stbi_image_free(first);
    }

    size_t layerSize = static_cast<size_t>(mWidth) * mHeight * mNrChannels;

    mData.reserve(layerSize * (mLayerCount + newLayers));

    for (size_t i = 0; i < filepaths.size(); i++)
    {
        int w, h, c;
        unsigned char* data = stbi_load(filepaths[i].c_str(), &w, &h, &c, 0);
        if (!data)
            return false;

        if (w != mWidth || h != mHeight || c != mNrChannels)
        {
            stbi_image_free(data);
            return false;
        }

        size_t newLayerIndex = mLayerCount;

        mLayerCount++;
        mData.resize(layerSize * mLayerCount);

        std::memcpy(mData.data() + newLayerIndex * layerSize, data, layerSize);

        stbi_image_free(data);
    }

    return true;
}

bool Image::LoadFromSpriteSheet(const std::string& filepath, int spriteWidth, int spriteHeight, bool flipVertically)
{
    if (spriteWidth <= 0 || spriteHeight <= 0)
        return false;

    stbi_set_flip_vertically_on_load(false);

    int width, height, channels;
    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
    if (!data)
        return false;

    if (width % spriteWidth != 0 || height % spriteHeight != 0)
    {
        stbi_image_free(data);
        return false;
    }

    mWidth = spriteWidth;
    mHeight = spriteHeight;
    mNrChannels = channels;

    int columns = width / spriteWidth;
    int rows = height / spriteHeight;

    mLayerCount = columns * rows;

    size_t layerSize = (size_t)spriteWidth * spriteHeight * channels;
    mData.resize(layerSize * mLayerCount);

    size_t layer = 0;

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < columns; x++) {

            for (int ty = 0; ty < spriteHeight; ty++) {
                int srcY = y * spriteHeight + ty;
                if (flipVertically)
                    srcY = y * spriteHeight + (spriteHeight - 1 - ty);

                int srcX = x * spriteWidth;

                memcpy(
                    &mData[layer * layerSize + ty * spriteWidth * channels],
                    &data[(srcY * width + srcX) * channels],
                    spriteWidth * channels
                );
            }

            layer++;
        }
    }

    stbi_image_free(data);
    return true;
}

bool Image::LoadFromSpriteSheets(const std::vector<std::string>& filepaths, int spriteWidth, int spriteHeight, bool flipVertically)
{
    if (filepaths.empty())
        return false;

    stbi_set_flip_vertically_on_load(false);

    int channels = 0;
    mLayerCount = 0;

    // -------- PASS 1: VALIDIEREN --------
    for (const auto& path : filepaths) {
        int w, h, c;
        unsigned char* data = stbi_load(path.c_str(), &w, &h, &c, 0);
        if (!data)
            return false;

        if (w % spriteWidth != 0 || h % spriteHeight != 0) {
            stbi_image_free(data);
            return false;
        }

        if (mLayerCount == 0)
            channels = c;
        else if (c != channels) {
            stbi_image_free(data);
            return false;
        }

        mLayerCount += (w / spriteWidth) * (h / spriteHeight);

        stbi_image_free(data);
    }

    if (mLayerCount == 0)
        return false;

    mWidth = spriteWidth;
    mHeight = spriteHeight;
    mNrChannels = channels;

    size_t layerSize = (size_t)spriteWidth * spriteHeight * channels;
    mData.resize(layerSize * mLayerCount);

    size_t layer = 0;

    for (const auto& path : filepaths) {
        int w, h, c;
        unsigned char* data = stbi_load(path.c_str(), &w, &h, &c, 0);
        if (!data)
            continue;

        int tilesX = w / spriteWidth;
        int tilesY = h / spriteHeight;

        for (int y = 0; y < tilesY; y++) {
            for (int x = 0; x < tilesX; x++) {

                for (int ty = 0; ty < spriteHeight; ty++) {
                    int srcY = y * spriteHeight + ty;
                    if (flipVertically)
                        srcY = y * spriteHeight + (spriteHeight - 1 - ty);

                    int srcX = x * spriteWidth;

                    memcpy(
                        &mData[layer * layerSize + ty * spriteWidth * channels],
                        &data[(srcY * w + srcX) * channels],
                        spriteWidth * channels
                    );
                }

                layer++;
            }
        }

        stbi_image_free(data);
    }

    return true;
}

bool Image::AddLayersFromSpriteSheet(const std::string& filepath, int spriteWidth, int spriteHeight, bool flipVertically)
{
    stbi_set_flip_vertically_on_load(false);

    int width, height, channels;
    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
    if (!data)
        return false;

    if (width % spriteWidth != 0 || height % spriteHeight != 0) {
        stbi_image_free(data);
        return false;
    }

    if (mLayerCount > 0 &&
        (spriteWidth != mWidth || spriteHeight != mHeight || channels != mNrChannels))
    {
        stbi_image_free(data);
        return false;
    }

    if (mLayerCount == 0) {
        mWidth = spriteWidth;
        mHeight = spriteHeight;
        mNrChannels = channels;
    }

    int columns = width / spriteWidth;
    int rows = height / spriteHeight;
    int newLayers = columns * rows;

    size_t layerSize = (size_t)mWidth * mHeight * mNrChannels;

    size_t oldLayerCount = mLayerCount;
    mLayerCount += newLayers;

    mData.resize(layerSize * mLayerCount);

    size_t layer = oldLayerCount;

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < columns; x++) {

            for (int ty = 0; ty < spriteHeight; ty++) {
                int srcY = y * spriteHeight + ty;
                if (flipVertically)
                    srcY = y * spriteHeight + (spriteHeight - 1 - ty);

                int srcX = x * spriteWidth;

                memcpy(
                    &mData[layer * layerSize + ty * spriteWidth * channels],
                    &data[(srcY * width + srcX) * channels],
                    spriteWidth * channels
                );
            }

            layer++;
        }
    }

    stbi_image_free(data);
    return true;
}

bool Image::AddLayersFromSpriteSheets(const std::vector<std::string>& filepaths, int spriteWidth, int spriteHeight, bool flipVertically)
{
    if (filepaths.empty())
        return false;

    for (const auto& path : filepaths) {
        if (!AddLayersFromSpriteSheet(path, spriteWidth, spriteHeight, flipVertically))
            return false;
    }

    return true;
}

bool Image::SaveAs(const char* filepath, bool flipVertically)
{
    if (mData.empty() || mWidth <= 0 || mHeight <= 0 || mNrChannels <= 0)
        return false;

    if (flipVertically)
        stbi_flip_vertically_on_write(1);
    else
        stbi_flip_vertically_on_write(0);

    const unsigned char* data = mData.data();

    int success = 0;

    std::string path(filepath);

    // PNG
    if (path.size() >= 4 && path.substr(path.size() - 4) == ".png")
    {
        success = stbi_write_png(
            filepath,
            mWidth,
            mHeight,
            mNrChannels,
            data,
            mWidth * mNrChannels
        );
    }
    // JPG / JPEG
    else if (path.size() >= 4 &&
        (path.substr(path.size() - 4) == ".jpg" ||
            path.substr(path.size() - 5) == ".jpeg"))
    {
        success = stbi_write_jpg(
            filepath,
            mWidth,
            mHeight,
            mNrChannels,
            data,
            90 // quality
        );
    }
    // BMP
    else if (path.size() >= 4 && path.substr(path.size() - 4) == ".bmp")
    {
        success = stbi_write_bmp(
            filepath,
            mWidth,
            mHeight,
            mNrChannels,
            data
        );
    }
    else
    {
        return false; // unsupported format
    }

    return success != 0;
}

bool Image::LoadFromTexture(const Texture& texture)
{
    Unload();

    if (!texture.GetData(mData))
        return false;

    mWidth = texture.GetWidth();
    mHeight = texture.GetHeight();
    mNrChannels = texture.GetNrChannels();
    mLayerCount = texture.GetLayerCount();

    return true;
}

void Image::Unload()
{
    mData.clear();

    mWidth = 0;
    mHeight = 0;
    mNrChannels = 0;
    mLayerCount = 0;
}

Color Image::GetPixel(int x, int y, int z)
{
    if (x < 0 || y < 0 || z < 0 ||
        x >= mWidth || y >= mHeight || z >= mLayerCount)
    {
        return Color(); // or assert / error
    }

    size_t layerSize = (size_t)mWidth * mHeight * mNrChannels;
    size_t rowSize = (size_t)mWidth * mNrChannels;

    size_t index = z * layerSize + y * rowSize + x * mNrChannels;

    unsigned char r = 0, g = 0, b = 0, a = 255;

    if (mNrChannels >= 1) r = mData[index + 0];
    if (mNrChannels >= 2) g = mData[index + 1];
    if (mNrChannels >= 3) b = mData[index + 2];
    if (mNrChannels >= 4) a = mData[index + 3];

    return Color(r, g, b, a);
}

bool Image::SetPixel(Color color, int x, int y, int z)
{
    if (x < 0 || y < 0 || z < 0 ||
        x >= mWidth || y >= mHeight || z >= mLayerCount)
        return false;

    size_t layerSize = (size_t)mWidth * mHeight * mNrChannels;
    size_t rowSize = (size_t)mWidth * mNrChannels;

    size_t index = z * layerSize + y * rowSize + x * mNrChannels;

    if (mNrChannels >= 1) mData[index + 0] = color.r;
    if (mNrChannels >= 2) mData[index + 1] = color.g;
    if (mNrChannels >= 3) mData[index + 2] = color.b;
    if (mNrChannels >= 4) mData[index + 3] = color.a;

    return true;
}

void Image::FillLayer(int layerIndex, Color color)
{
    if (layerIndex < 0 || layerIndex >= mLayerCount)
        return;

    size_t layerSize = (size_t)mWidth * mHeight * mNrChannels;
    unsigned char* layerPtr = mData.data() + layerIndex * layerSize;

    size_t pixelCount = (size_t)mWidth * mHeight;

    for (size_t i = 0; i < pixelCount * mNrChannels; i += mNrChannels)
    {
        if (mNrChannels >= 1) layerPtr[i + 0] = color.r;
        if (mNrChannels >= 2) layerPtr[i + 1] = color.g;
        if (mNrChannels >= 3) layerPtr[i + 2] = color.b;
        if (mNrChannels >= 4) layerPtr[i + 3] = color.a;
    }
}
