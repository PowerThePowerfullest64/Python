#pragma once

#include <type_traits>

#include "utils/math/vec2.hpp"
#include "utils/structures/line.hpp"

template<typename T>
requires (std::is_arithmetic_v<T>)
class Rectangle {
public:
    Vec2<T> pos;
    Vec2<T> size;

    Rectangle(const Vec2<T>& pos, const Vec2<T>& size) : pos(pos), size(size) {}
    Rectangle(T x, T y, T w, T h) : pos({x, y}), size({w, h}) {}

    template<typename U>
    requires std::is_constructible_v<U, T, T, T, T>
    // Convert to a struct that also takes 4 components as a valid constructor.
    operator U() const {
        return U{
            static_cast<decltype(U{}.x)>(pos.x),
            static_cast<decltype(U{}.y)>(pos.y),
            static_cast<decltype(U{}.w)>(size.x),
            static_cast<decltype(U{}.h)>(size.y)
        };
    }

    constexpr inline T Left() const {
        return pos.x;
    }
    constexpr inline T Right() const {
        return pos.x + size.x;
    }
    constexpr inline T Top() const {
        return pos.y;
    }
    constexpr inline T Bottom() const {
        return pos.y + size.y;
    }

    constexpr inline Line<T> LeftLine() const {
        return {{Left(), Top()}, {Left(), Bottom()}};
    }
    constexpr inline Line<T> RightLine() const {
        return {{Right(), Top()}, {Right(), Bottom()}};
    }
    constexpr inline Line<T> TopLine() const {
        return {{Left(), Top()}, {Right(), Top()}};
    }
    constexpr inline Line<T> BottomLine() const {
        return {{Left(), Bottom()}, {Right(), Bottom()}};
    }
};