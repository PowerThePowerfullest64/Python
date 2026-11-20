#pragma once

#include <cmath>
#include <iostream>
#include <format>
#include <concepts>
#include <type_traits>
#include <cassert>
#include <functional>
#include <cstddef>

#include "utils/math/constants.hpp"

template<typename T>
concept Number = std::is_arithmetic_v<T>;

template<typename T>
struct Vec2 {
    T x{}, y{};

    constexpr Vec2() = default;
    constexpr Vec2(T x, T y) : x(x), y(y) {}

    template<typename U>
    // Check if type can be constructed with 2 components.
    requires requires { U{x, y}; }
    // Convert to a struct that also takes 2 components as a valid constructor.
    operator U() const {
        return { x, y };
    }
    template<typename U>
    Vec2(const U& other)
        : x(other.x), y(other.y) {}

    template<typename U>
    // Try to implicitly convert other Vec2 type to this type.
    Vec2(const Vec2<U>& other)
        : x(static_cast<T>(other.x)),
          y(static_cast<T>(other.y)) {}

    Vec2 operator+(const Vec2& v) const requires Number<T> {
        return {x + v.x, y + v.y};
    }
    Vec2 operator-(const Vec2& v) const requires Number<T> {
        return {x - v.x, y - v.y};
    }
    Vec2 operator*(T scalar) const requires Number<T> {
        return {x * scalar, y * scalar};
    }
    Vec2 operator/(T den) const requires Number<T> {
        return {x / den, y / den};
    }
    Vec2 operator+=(const Vec2& v) requires Number<T> {
        x += v.x;
        y += v.y;
        return *this;
    }
    Vec2 operator-=(const Vec2& v) requires Number<T> {
        x -= v.x;
        y -= v.y;
        return *this;
    }
    Vec2 operator*=(T scalar) requires Number<T> {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    Vec2 operator/=(T den) requires Number<T> {
        x /= den;
        y /= den;
        return *this;
    }

    // Dot-product
    float operator*(const Vec2& v) const requires Number<T> {
        return x * v.x + y * v.y;
    }

    bool operator==(const Vec2& v) const {
        return x == v.x && y == v.y;
    }
    bool operator!=(const Vec2& v) const {
        return !(*this == v);
    }

    T Length() const requires std::is_floating_point_v<T> {
        return sqrt(x * x + y * y);
    }
    T LengthSq() const requires std::is_floating_point_v<T> {
        return x * x + y * y;
    }

    T DistanceTo(const Vec2& v) const requires std::is_floating_point_v<T> {
        Vec2 dv = v - *this;
        return dv.Length();
    }
    T DistanceToSq(const Vec2& v) const requires std::is_floating_point_v<T> {
        Vec2 dv = v - *this;
        return dv.LengthSq();
    }

    Vec2 Normalized() const requires std::is_floating_point_v<T> {
        float length = sqrtf(x * x + y * y);

        if (std::abs(length) <= 0.0001f)
            return {0, 0};

        return *this / length;
    }
    void Normalize() requires std::is_floating_point_v<T> {
        float length = sqrtf(x * x + y * y);

        if (std::abs(length) <= 0.0001f) {
            *this = {0, 0};
            return;
        }
        
        *this /= length;
    }
    static Vec2<T> Lerp(const Vec2<T>& a, const Vec2<T>& b, T t) requires std::is_floating_point_v<T> {
        return a + (b - a) * t;
    }

    T AngleR() const requires std::is_floating_point_v<T> {
        return std::atan2(y, x);
    }
    T AngleD() const requires std::is_floating_point_v<T> {
        return std::atan2(y, x) * (180 / PI);
    }

    static Vec2<T> FromAngleR(T angle) {
        T _x = std::cos(angle);
        T _y = std::sin(angle);

        return {_x, _y};
    }
    static Vec2<T> FromAngleD(T angle) {
        angle *= PI / 180; // Convert to radians.

        T _x = std::cos(angle);
        T _y = std::sin(angle);

        return {_x, _y};
    }
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Vec2<T>& v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

template<typename T>
struct std::formatter<Vec2<T>> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const Vec2<T>& v, format_context& ctx) const {
        return std::format_to(ctx.out(), "({}, {})", v.x, v.y);
    }
};

namespace std {
    template <typename T>
    struct hash<Vec2<T>> {
        std::size_t operator()(const Vec2<T>& v) const noexcept {
            std::size_t h1 = std::hash<T>{}(v.x);
            std::size_t h2 = std::hash<T>{}(v.y);
            return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
        }
    };
}