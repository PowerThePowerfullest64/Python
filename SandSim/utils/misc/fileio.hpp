#pragma once

#include <type_traits>
#include <optional>
#include <cstddef>

namespace FileIO {
    // Write a single trivially-copyable value. Returns true on success.
    template<typename T>
    requires (std::is_trivially_copyable_v<T> && !std::is_pointer_v<T>)
    bool Write(std::ostream& file, const T& data) {
        file.write(reinterpret_cast<const char*>(&data), sizeof(T));
        return file.good();
    }

    // Write 'count' trivially-copyable elements. Returns true on success.
    template<typename T>
    requires (std::is_trivially_copyable_v<T>)
    bool Write(std::ostream& file, const T* ptr, std::size_t count) {
        file.write(reinterpret_cast<const char*>(ptr), sizeof(T) * count);
        return file.good();
    }

    // Read a single trivially-copyable value. Returns the read value on success.
    template<typename T>
    requires (std::is_trivially_copyable_v<T> && !std::is_pointer_v<T>)
    std::optional<T> Read(std::istream& file) {
        T value;
        file.read(reinterpret_cast<char*>(&value), sizeof(T));
        
        if (!file.good())
            return std::nullopt;
        
        return value;
    }

    // Read 'count' trivially-copyable elements. The read value is written to out.
    template<typename T>
    requires (std::is_trivially_copyable_v<T>)
    bool Read(std::istream& file, T* out, std::size_t count) {
        file.read(reinterpret_cast<char*>(out), sizeof(T) * count);
        return file.good();
    }
}
