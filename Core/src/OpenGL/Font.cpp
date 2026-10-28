#include "OpenGL/Font.h"

#include <glad/glad.h>

#include <filesystem>

#include <ft2build.h>
#include FT_FREETYPE_H

uint16_t allowedSizes[] = { 12, 16, 24, 32, 48, 64, 96, 128 };

bool Font::LoadFromFile(const std::string& filepath, bool pixelArt)
{
    mSizes.clear();

    if (!std::filesystem::exists(filepath)) return false;

    mFilepath = filepath;
    mPixelArt = pixelArt;
    return true;
}

bool Font::Load(uint16_t size)
{
    mSizes.erase(size);

    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library\n";
        return false;
    }

    FT_Face face;
    if (FT_New_Face(ft, mFilepath.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font: " << mFilepath << "\n";
        FT_Done_FreeType(ft);
        return false;
    }

    FT_Set_Pixel_Sizes(face, 0, size);

    // 1. Maximalgröße berechnen & Gesamtbreite summieren
    int maxHeight = 0;
    int totalWidth = 0;
    int glyphWidths[95] = {};

    for (unsigned char c = 32; c <= 126; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) continue;
        int idx = c - 32;
        glyphWidths[idx] = face->glyph->bitmap.width;
        totalWidth += face->glyph->bitmap.width;
        maxHeight = std::max(maxHeight, (int)face->glyph->bitmap.rows);
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // 2. Texture Array mit 1 Layer erstellen
    GLuint texID;
    glGenTextures(1, &texID);

    mSizes[size].textureArray.Create(texID, totalWidth, maxHeight, 1, 1);
    mSizes[size].textureArray.Bind();
    mSizes[size].size = size;
    mSizes[size].atlasWidth = totalWidth;
    mSizes[size].atlasHeight = maxHeight;

    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RED,
        totalWidth, maxHeight, 1,
        0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

    if (mPixelArt) {
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    else {
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    float maxAscender = 0.0f;
    int xOffset = 0;

    // 3. Glyphen horizontal nebeneinander in Layer 0 schreiben
    for (unsigned char c = 32; c <= 126; c++) {
        if (FT_Load_Char(face, c, mPixelArt
            ? FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT
            : FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYPE: Failed to load Glyph " << c << "\n";
            continue;
        }

        int idx = c - 32;
        int w = face->glyph->bitmap.width;
        int h = face->glyph->bitmap.rows;

        // Zeilenweise kopieren (pitch != width möglich)
        std::vector<unsigned char> buffer(w * h, 0);
        for (int row = 0; row < h; row++) {
            memcpy(
                buffer.data() + row * w,
                face->glyph->bitmap.buffer + row * face->glyph->bitmap.pitch,
                w
            );
        }

        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,
            xOffset, 0, 0,       // x, y, layer
            w, h, 1,
            GL_RED, GL_UNSIGNED_BYTE,
            buffer.data());

        mSizes[size].glyphs[idx] = {
            (unsigned int)w, (unsigned int)h,
            face->glyph->bitmap_left,
            face->glyph->bitmap_top,
            face->glyph->advance.x >> 6
        };

        // UV in [0,1] relativ zur Atlas-Breite
        mSizes[size].uvs[idx] = {
            float(xOffset) / float(totalWidth),
            float(xOffset + w) / float(totalWidth)
        };

        maxAscender = std::max(maxAscender, float(face->glyph->bitmap_top));
        xOffset += w;
    }

    mSizes[size].ascender = maxAscender;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    return true;
}

float Font::GetTextLength(uint16_t size, std::string& text)
{
    return GetTextLength(size, text.data());
}

float Font::GetTextLength(uint16_t size, const char* text)
{
    FontSizeData* data = GetFontSizeData(size);
    float scale = float(size) / float(data->size);

    float maxAdvance = 0.0f;
    float lineAdvance = 0.0f;

    const char* ptr = text;
    while (*ptr != '\0') {
        char c = *ptr++;

        if (c == '\n') {
            maxAdvance = std::max(maxAdvance, lineAdvance);
            lineAdvance = 0.0f;
            continue;
        }

        if (c < 32 || c > 126)
            continue;

        uint16_t index = static_cast<uint16_t>(c) - 32;
        Glyph& glyph = data->glyphs[index];

        lineAdvance += glyph.advance * scale;
    }

    maxAdvance = std::max(maxAdvance, lineAdvance);

    return maxAdvance;
}

FontSizeData* Font::GetFontSizeData(uint16_t size) {

    auto it = mSizes.find(size);
    if (it != mSizes.end()) {
        return &it->second;
    }
    uint16_t allowedSize = ResolveAllowedSize(size);

    it = mSizes.find(allowedSize);
    if (it != mSizes.end()) {
        return &it->second;
    }

    if (Load(allowedSize)) 
        return &mSizes.at(allowedSize);

    return nullptr;
}

uint16_t Font::ResolveAllowedSize(uint16_t requested) {
    uint16_t best = allowedSizes[0];

    for (uint16_t s : allowedSizes) {
        if (s >= requested) {
            return s;
        }
        best = s;
    }

    return best;
}