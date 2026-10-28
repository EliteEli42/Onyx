#pragma once

#include <Core/Common/MathTypes.h>
#include <Core/OpenGL/Color.h>

#include <string>
#include <vector>

namespace Core {

    class Texture;

    class Image {
    public:
        void CreateSolidColor(int width, int height, int nrChannels, int layerCount, Color color);
        void AddSolidColorLayer(Color color);

        void CreateEmpty(int width, int height, int nrChannels);
        bool LoadFromMemory(const unsigned char* data, int width, int height, int nrChannels, int LayerCount);

        bool LoadFromFile(const std::string& filepath, bool flipVertically = false);
        bool LoadFromFiles(const std::vector<std::string>& filepaths, bool flipVertically = false);

        bool AddLayerFromFile(const std::string& filepath, bool flipVertically = false);
        bool AddLayersFromFiles(const std::vector<std::string>& filepaths, bool flipVertically = false);

        bool LoadFromSpriteSheet(const std::string& filepath, int spriteWidth, int spriteHeight, bool flipVertically = false);
        bool LoadFromSpriteSheets(const std::vector<std::string>& filepaths, int spriteWidth, int spriteHeight, bool flipVertically = false);

        bool AddLayersFromSpriteSheet(const std::string& filepath, int spriteWidth, int spriteHeight, bool flipVertically = false);
        bool AddLayersFromSpriteSheets(const std::vector<std::string>& filepaths, int spriteWidth, int spriteHeight, bool flipVertically = false);

        bool SaveAs(const char* filepath, bool flipVertically = false);

        bool LoadFromTexture(const Texture& texture);

        void Unload();
        ~Image() {
            Unload();
        }

        Color GetPixel(int x, int y, int z = 0);
        bool SetPixel(Color color, int x, int y, int z = 0);

        void FillLayer(int layerIndex, Color color);

        const unsigned char* GetLayer(int index) const {
            return mData.data() + index * mWidth * mHeight * mNrChannels;
        }
        unsigned char* GetLayer(int index) {
            return mData.data() + index * mWidth * mHeight * mNrChannels;
        }

        std::vector<unsigned char>& GetData() { return mData; }
        const std::vector<unsigned char>& GetData() const { return mData; }
        inline int GetWidth() const { return mWidth; }
        inline int GetHeight() const { return mHeight; }
        inline IVec2 GetSize() const { return IVec2(mWidth, mHeight); }
        inline int GetNrChannels() const { return mNrChannels; }
        inline int GetLayerCount() const { return mLayerCount; }

    private:
        std::vector<unsigned char> mData;

        int mWidth = 0;
        int mHeight = 0;
        int mNrChannels = 0;
        int mLayerCount = 0;
    };

} // namespace Core