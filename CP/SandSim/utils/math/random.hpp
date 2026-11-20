#pragma once

#include <random>
#include <concepts>
#include <array>
#include <vector>
#include <cassert>
#include <cstdint>

namespace Random {
    // Returns a random floating-point number between min (inclusive) and max (inclusive). Use RangeI() for integers instead.
    template<typename T>
    requires std::floating_point<T>
    inline T Range(T min = 0, T max = 1) {
        static std::mt19937 rng(std::random_device{}());
        std::uniform_real_distribution<T> dist(min, max);
        return dist(rng);
    }

    // Returns a random integer 32 between min (inclusive) and max (inclusive).
    inline int32_t RangeI32(int32_t min = 0, int32_t max = 1) {
        static std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int32_t> dist(min, max);
        return dist(rng);
    }

    // Returns a random integer 64 between min (inclusive) and max (inclusive).
    inline int64_t RangeI64(int64_t min = 0, int64_t max = 1) {
        static std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int64_t> dist(min, max);
        return dist(rng);
    }

    template<typename T, size_t size>
    requires (size > 0)
    inline T& Choice(std::array<T, size>& a) {
        return a[RangeI32(0, size-1)];
    }

    template<typename T, size_t size>
    requires (size > 0)
    inline T& Choice(T (&a)[size]) {
        return a[RangeI32(0, size-1)];
    }

    template<typename T>
    inline T& Choice(std::vector<T>& v) {
        assert(!v.empty() && "Cannot randomly choose an element from an empty vector.");
        return v[RangeI32(0, v.size()-1)];
    }
}
