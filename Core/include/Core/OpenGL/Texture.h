#pragma once

#include <Common/MathTypes.h>
#include <vector>
#include <string>
#include <iostream>

using TextureID = uint32_t;

enum class TextureFilter {
	Nearest = 0x2600,
	Linear = 0x2601,
	NearestMipmapNearest = 0x2700,
	LinearMipmapNearest = 0x2701,
	NearestMipmapLinear = 0x2702,
	LinearMipmapLinear = 0x2703
};

enum class TextureWrap {
	Repeat = 0x2901,
	MirroredRepeat = 0x8370,
	ClampToEdge = 0x812F,
	ClampToBorder = 0x812D
};

class Image;

class Texture {
public:
	void Create(TextureID ID, int Width, int Height, int NrChannels, int LayerCount) {
		mID = ID;
		mWidth = Width;
		mHeight = Height;
		mNrChannels = NrChannels;
		mLayerCount = LayerCount;
	}

	bool CreateEmpty(
		int width, int height, int nrChannels,
		TextureFilter minFilter = TextureFilter::Linear,
		TextureFilter magFilter = TextureFilter::Linear,
		TextureWrap wrapS = TextureWrap::ClampToEdge,
		TextureWrap wrapT = TextureWrap::ClampToEdge
	);

	bool LoadFromMemory(
		const unsigned char* data,
		int width,
		int height,
		int nrChannels,
		int LayerCount,
		TextureFilter minFilter = TextureFilter::Nearest,
		TextureFilter magFilter = TextureFilter::Nearest,
		TextureWrap wrapS = TextureWrap::Repeat,
		TextureWrap wrapT = TextureWrap::Repeat
	);

	bool LoadFromFile(
		const std::string& filepath,
		bool flipVertically = false,
		TextureFilter minFilter = TextureFilter::Nearest,
		TextureFilter magFilter = TextureFilter::Nearest,
		TextureWrap wrapS = TextureWrap::Repeat,
		TextureWrap wrapT = TextureWrap::Repeat
	);

	bool LoadFromFiles(
		const std::vector<std::string>& filepaths,
		bool flipVertically = false,
		TextureFilter minFilter = TextureFilter::Nearest,
		TextureFilter magFilter = TextureFilter::Nearest,
		TextureWrap wrapS = TextureWrap::Repeat,
		TextureWrap wrapT = TextureWrap::Repeat
	);

	bool LoadFromSpriteSheet(
		const std::string& filepath,
		int spriteWidth,
		int spriteHeight,
		bool flipVertically = false,
		TextureFilter minFilter = TextureFilter::Nearest,
		TextureFilter magFilter = TextureFilter::Nearest,
		TextureWrap wrapS = TextureWrap::Repeat,
		TextureWrap wrapT = TextureWrap::Repeat
	);

	bool LoadFromSpriteSheets(
		const std::vector<std::string>& filepaths,
		int spriteWidth,
		int spriteHeight,
		bool flipVertically = false,
		TextureFilter minFilter = TextureFilter::Nearest,
		TextureFilter magFilter = TextureFilter::Nearest,
		TextureWrap wrapS = TextureWrap::Repeat,
		TextureWrap wrapT = TextureWrap::Repeat
	);

	bool LoadFromImage(const Image& image,
		TextureFilter minFilter = TextureFilter::Nearest,
		TextureFilter magFilter = TextureFilter::Nearest,
		TextureWrap wrapS = TextureWrap::Repeat,
		TextureWrap wrapT = TextureWrap::Repeat
	);

	void Unload();
	~Texture() {
		Unload();
	}

	// DEFAULT-KONSTRUKTOR
	Texture() = default;

	// COPY-KONSTRUKTOR & COPY-ASSIGNMENT (deleted)
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	// MOVE-KONSTRUKTOR
	Texture(Texture&& other) noexcept {
		MoveFrom(std::move(other));
	}

	// MOVE-ASSIGNMENT
	Texture& operator=(Texture&& other) noexcept {
		if (this != &other) {
			Unload();
			MoveFrom(std::move(other));
		}
		return *this;
	}

	void Bind(TextureID unit = 0) const;

	bool GetData(std::vector<unsigned char>& outData) const;
	inline TextureID GetID() const { return mID; }
	inline int GetWidth() const { return mWidth; }
	inline int GetHeight() const { return mHeight; }
	inline IVec2 GetSize() const { return IVec2(mWidth, mHeight); }
	inline int GetNrChannels() const { return mNrChannels; }
	inline int GetLayerCount() const { return mLayerCount; }

private:
	TextureID mID = 0;
	int mWidth = 0;
	int mHeight = 0;
	int mNrChannels = 0;
	int mLayerCount = 0;

	void MoveFrom(Texture&& other);
};