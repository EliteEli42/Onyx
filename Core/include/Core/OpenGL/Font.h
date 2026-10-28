#pragma once

#include "Texture.h"
#include <unordered_map>

struct Glyph {
	unsigned int width, height;
	int bearingX, bearingY;
	int advance;
};

struct FontSizeData {
	uint16_t size;
	float ascender;
	Texture textureArray; // 1 Layer
	Glyph glyphs[95];
	Vec2 uvs[95];      // UV-X-Bereich pro Glyph im Atlas
	int atlasWidth = 0;
	int atlasHeight = 0;
};

class Font {
public:
	bool LoadFromFile(const std::string& filepath, bool pixelArt = false);
	bool Load(uint16_t size);

	float GetTextLength(uint16_t size, std::string& text);
	float GetTextLength(uint16_t size, const char* text);

	FontSizeData* GetFontSizeData(uint16_t size);
	std::string GetFilePath() { return mFilepath; }

private:
	uint16_t ResolveAllowedSize(uint16_t requested);

	std::unordered_map<uint16_t, FontSizeData> mSizes;
	std::string mFilepath;
	bool mPixelArt = false;
};