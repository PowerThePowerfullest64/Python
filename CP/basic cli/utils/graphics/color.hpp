#pragma once

#include <cstdint>

struct Color {
    std::uint8_t r, g, b, a;

    constexpr Color(
        std::uint8_t r = 0,
        std::uint8_t g = 0,
        std::uint8_t b = 0,
        std::uint8_t a = 255
    ) : r(r), g(g), b(b), a(a) {}

    constexpr Color(const Color& color) : r(color.r), g(color.g), b(color.b), a(color.a) {}

    constexpr Color operator+(int x) {
        Color c;
        c.r = (*this).r + x;
        c.g = (*this).g + x;
        c.b = (*this).b + x;
        //c.a = (*this).a + x;
        return  c;
    }

    constexpr void operator+=(int x) {
        (*this).r += x;
        (*this).g += x;
        (*this).b += x;
        //(*this).a += x;
    }

    constexpr Color operator-(int x) {
        Color c;
        c.r = (*this).r - x;
        c.g = (*this).g - x;
        c.b = (*this).b - x;
        //c.a = (*this).a - x;
        return  c;
    }

    constexpr void operator-=(int x) {
        (*this).r -= x;
        (*this).g -= x;
        (*this).b -= x;
        //(*this).a -= x;
    }
};

namespace Colors {
    constexpr Color WHITE = {255, 255, 255};
    constexpr Color BLACK = {0, 0, 0};

    constexpr Color RED = {255, 0, 0};
    constexpr Color GREEN = {0, 255, 0};
    constexpr Color BLUE = {0, 0, 255};

    constexpr Color LIGHT_GRAY = {186, 186, 186};
    constexpr Color GRAY = {127, 127, 127};
    constexpr Color DARK_GRAY = {60, 60, 60};

    constexpr Color YELLOW = {252, 232, 3};
    constexpr Color PURPLE = {145, 0, 230};
    constexpr Color ORANGE = {255, 174, 0};
    constexpr Color CYAN = {0, 255, 255};
    constexpr Color PINK = {255, 0, 195};

    constexpr Color DARK_RED = {105, 0, 0};
    constexpr Color LIGHT_RED = {255, 77, 77};
    constexpr Color DARK_GREEN = {0, 97, 0};
    constexpr Color LIGHT_GREEN = {89, 255, 89};
    constexpr Color DARK_BLUE = {10, 23, 122};
    constexpr Color LIGHT_BLUE = {59, 85, 255};

    constexpr Color BLANK = {0, 0, 0, 0};
}
