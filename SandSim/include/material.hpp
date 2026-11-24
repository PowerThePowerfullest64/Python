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
    WATER,
    STEEL,
    WOOD,
    SMOKE,
    STEAM,
    COUNT,
    NONE
};

enum class MaterialState : uint8_t {
    LIQUID,
    SOLID,
    GAS,
    STATIC,
    AIR
};

struct MaterialDef {
    std::string_view name;
    MaterialState state;
    int16_t lifetime;
    MaterialId product;
    int8_t density;
    std::array<uint32_t, 4> palette;
    uint8_t paletteSize;
};

// List of the properties of each material id.
constexpr std::array<MaterialDef, static_cast<size_t>(MaterialId::COUNT)> materials {{
    {"air", MaterialState::AIR, -1, MaterialId::NONE, 0, {0x00000000}, 1},
    {"sand", MaterialState::SOLID, -1, MaterialId::NONE, 10, {MakeColor(232, 206, 111), MakeColor(207, 184, 101)}, 2},
    {"water", MaterialState::LIQUID, -1, MaterialId::NONE, 8, {MakeColor(17, 49, 214)}, 1},
    {"steel", MaterialState::STATIC, -1, MaterialId::NONE, 20, {MakeColor(72, 90, 140)}, 1},
    {"wood", MaterialState::STATIC, -1, MaterialId::NONE, 5, {MakeColor(75, 54, 27), MakeColor(66, 46, 17)}, 2},
    {"smoke", MaterialState::GAS, 4000, MaterialId::AIR, -5, {MakeColor(58, 60, 71), MakeColor(52, 54, 64)}, 2},
    {"steam", MaterialState::GAS, 2500, MaterialId::WATER, -10, {MakeColor(187, 186, 245)}, 1}
}};

inline const MaterialDef& GetMaterialDef(MaterialId material) {
    return materials[static_cast<size_t>(material)];
}
