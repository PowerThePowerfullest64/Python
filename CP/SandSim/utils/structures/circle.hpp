#pragma once

#include <type_traits>

#include "utils/math/vec2.hpp"

template<typename T>
requires (std::is_arithmetic_v<T>)
class Circle {
public:
    Vec2<T> pos;
    T radius = 0;

    Circle(const Vec2<T>& pos, T radius) : pos(pos), radius(radius) {}
    Circle(T x, T y, T radius) : pos({x, y}), radius(radius) {}
};