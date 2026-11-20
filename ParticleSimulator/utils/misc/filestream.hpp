#pragma once

#include <fstream>
#include <concepts>

template<typename T>
requires (std::is_trivially_copyable_v<T>)
// Write binary data of trivially-copyable types.
void Write(std::ofstream& file, T& data) {
    file.write(reinterpret_cast<const char*>(&data), sizeof(T));
}

template<typename T>
requires (std::is_trivially_copyable_v<T>)
// Read binary data of trivially-copyable types.
void Read(std::ifstream& file, T& out) {
    file.read(reinterpret_cast<char*>(&out), sizeof(T));
}