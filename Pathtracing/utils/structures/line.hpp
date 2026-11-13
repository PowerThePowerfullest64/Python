#pragma once

#include <type_traits>

#include "utils/math/vec2.hpp"

template<typename T>
requires (std::is_arithmetic_v<T>)
class Line {
public:
    Vec2<T> from;
    Vec2<T> to;

    Line(const Vec2<T>& from, const Vec2<T>& to) : from(from), to(to) {}
    Line(T x1, T y1, T x2, T y2) : from({x1, y1}), to({x2, y2}) {}

    float Length() const { return from.DistanceTo(to); }
};