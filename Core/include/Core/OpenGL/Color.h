#pragma once
#include <Common/MathTypes.h>

struct Color {
    uint8_t r, g, b, a;

    constexpr Color(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0, uint8_t alpha = 255)
        : r(red), g(green), b(blue), a(alpha) {
    }

    constexpr Vec4 ToNormalizedVec4() const
    {
        return Vec4(
            float(r) / 255.0f,
            float(g) / 255.0f,
            float(b) / 255.0f,
            float(a) / 255.0f
        );
    }

    static constexpr Color FromNormalizedVec4(Vec4 vec)
    {
        return Color(
            uint8_t(vec.x * 255.0f),
            uint8_t(vec.y * 255.0f),
            uint8_t(vec.z * 255.0f),
            uint8_t(vec.w * 255.0f)
        );
    }
};