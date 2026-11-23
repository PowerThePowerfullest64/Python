#pragma once

#include <array>
#include <cstdint>
#include <string_view>

constexpr uint32_t MakeColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a=255) {
    return (static_cast<uint32_t>(a) << 24) |
           (static_cast<uint32_t>(b) << 16) |
           (static_cast<uint32_t>(g) << 8)  |
            static_cast<uint32_t>(r);
}

// Id of the material, to identify its properties and stuff.
enum class MaterialId : uint8_t {
    AIR,
    SAND,
    COUNT
};

struct MaterialDef {
    std::string_view name;
    std::array<uint32_t, 4> palette;
    uint8_t paletteSize;
};

// List of the properties of each material id.
constexpr std::array<MaterialDef, static_cast<size_t>(MaterialId::COUNT)> materials {{
    {"air", {0x00000000}, 1},
    {"sand", {MakeColor(255, 255, 0), MakeColor(246, 222, 0), MakeColor(244, 240, 14)}, 3}
}};

inline const MaterialDef& GetMaterialDef(MaterialId material) {
    return materials[static_cast<size_t>(material)];
}
