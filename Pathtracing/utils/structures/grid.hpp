#pragma once

#include <array>
#include <cassert>
#include <utility>

#include "utils/math/vec2.hpp"

template<typename T, size_t W, size_t H>
struct Grid {
    std::array<T, W * H> data = {};

    const size_t WIDTH = W;
    const size_t HEIGHT = H;

    // Looks up an element with bounds check.
    T& operator[](size_t idx) {
        assert(idx < W * H && "Accessed index outside range.");
        
        return data[idx];
    }
    T& operator[](int x, int y) {
        assert(x >= 0 && x < W && "Accessed index outside range.");
        assert(y >= 0 && y < H && "Accessed index outside range.");
        
        return data[Index(x, y)];
    }
    T& operator[](const Vec2<int>& v) {
        assert(v.x >= 0 && v.x < W && "Accessed index outside range.");
        assert(v.y >= 0 && v.y < H && "Accessed index outside range.");

        return data[Index(v)];
    }
    const T& operator[](size_t idx) const {
        assert(idx < W * H && "Accessed index outside range.");

        return data[idx];
    }
    const T& operator[](int x, int y) const {
        assert(x >= 0 && x < W && "Accessed index outside range.");
        assert(y >= 0 && y < H && "Accessed index outside range.");

        return data[Index(x, y)];
    }
    const T& operator[](const Vec2<int>& v) const {
        assert(v.x >= 0 && v.x < W && "Accessed index outside range.");
        assert(v.y >= 0 && v.y < H && "Accessed index outside range.");

        return data[Index(v)];
    }
    // Looks up an element without bounds check.
    T& operator()(size_t idx) {
        return data[idx];
    }
    T& operator()(int x, int y) {
        return data[Index(x, y)];
    }
    T& operator()(const Vec2<int>& v) {
        return data[Index(v)];
    }
    const T& operator()(size_t idx) const {
        return data[idx];
    }
    const T& operator()(int x, int y) const {
        return data[Index(x, y)];
    }
    const T& operator()(const Vec2<int>& v) const {
        return data[Index(v)];
    }
    
    // Changes all data within the grid to a designated value.
    void Fill(const T& value) {
        for (size_t i = 0; i < W * H; ++i)
            data[i] = value;
    }
    // Whether a position is within this grid's coordinates.
    bool Within(const Vec2<int>& v) const {
        return v.x >= 0 && v.x < WIDTH && v.y >= 0 && v.y < HEIGHT;
    }
    // Whether a certain value is contained within the grid.
    bool Contains(const T& value) const {
        return std::find(data.begin(), data.end(), value) != data.end();
    }
    void Swap(Grid& other) {
        std::swap(data, other.data);
    }
    constexpr bool Empty() const {
        return data.size() == 0;
    }
    constexpr size_t Size() const {
        return data.size();
    }

    constexpr int Index(const Vec2<int>& v) const {
        return v.y * W + v.x;
    }
    constexpr int Index(int x, int y) const {
        return y * W + x;
    }

    auto begin() { return data.begin(); }
    auto end() { return data.end(); }
    auto begin() const { return data.begin(); }
    auto end() const { return data.end(); }
};