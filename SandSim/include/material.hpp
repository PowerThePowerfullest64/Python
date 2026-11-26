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
    MOLTEN_STEEL,
    WOOD,
    SMOKE,
    STEAM,
    FIRE,
    GLASS,
    MOLTEN_GLASS,
    STOPPIUM,
    VOIDIUM,
    COUNT,
    NONE,
    ANY
};

enum class MaterialState : uint8_t {
    LIQUID,
    SOLID,
    GAS,
    STATIC,
    AIR
};

struct Reaction {
    MaterialId neighbor; // Needed to trigger reaction.
    MaterialId selfBecomes;
    MaterialId otherBecomes;
    uint16_t chance;
};

struct MaterialDef {
    std::string_view name;
    MaterialState state;
    int16_t lifetime;
    MaterialId product;
    int8_t density;
    std::array<uint32_t, 4> palette;
    uint8_t paletteSize;
    std::array<Reaction, 8> reactions{};
    uint8_t reactionsSize = 0;
};

// List of the properties of each material id.
constexpr std::array<MaterialDef, static_cast<size_t>(MaterialId::COUNT)> materials {{
    {"air", MaterialState::AIR, -1, MaterialId::NONE, 0, {0x00000000}, 1},
    {"sand", MaterialState::SOLID, -1, MaterialId::NONE, 10, {MakeColor(232, 206, 111), MakeColor(207, 184, 101)}, 2},
    {"water", MaterialState::LIQUID, -1, MaterialId::NONE, 8, {MakeColor(17, 49, 214)}, 1, {Reaction{MaterialId::MOLTEN_GLASS, MaterialId::STEAM, MaterialId::GLASS, 500}, Reaction{MaterialId::MOLTEN_STEEL, MaterialId::STEAM, MaterialId::STEEL, 100}}, 2},
    {"steel", MaterialState::STATIC, -1, MaterialId::NONE, 20, {MakeColor(72, 90, 140)}, 1},
    {"molten steel", MaterialState::LIQUID, 2500, MaterialId::STEEL, 30, {MakeColor(255, 115, 0), MakeColor(255, 166, 0)}, 2},
    {"wood", MaterialState::STATIC, -1, MaterialId::NONE, 5, {MakeColor(75, 54, 27), MakeColor(66, 46, 17)}, 2},
    {"smoke", MaterialState::GAS, 4000, MaterialId::AIR, -5, {MakeColor(58, 60, 71), MakeColor(52, 54, 64)}, 2},
    {"steam", MaterialState::GAS, 2500, MaterialId::WATER, -10, {MakeColor(187, 186, 245), MakeColor(180, 180, 240), MakeColor(175, 170, 250)}, 3},
    {"fire", MaterialState::GAS, 500, MaterialId::SMOKE, -15, {MakeColor(255, 217, 0), MakeColor(255, 162, 0), MakeColor(255, 94, 0)}, 3, {Reaction{MaterialId::WOOD, MaterialId::FIRE, MaterialId::FIRE, 50}, Reaction{MaterialId::SAND, MaterialId::SMOKE, MaterialId::MOLTEN_GLASS, 20}, Reaction{MaterialId::WATER, MaterialId::SMOKE, MaterialId::STEAM, 100}, Reaction{MaterialId::GLASS, MaterialId::SMOKE, MaterialId::MOLTEN_GLASS, 20}, Reaction{MaterialId::STEEL, MaterialId::SMOKE, MaterialId::MOLTEN_STEEL, 12}}, 5},
    {"glass", MaterialState::STATIC, -1, MaterialId::NONE, 15, {MakeColor(129, 170, 199), MakeColor(132, 170, 196)}, 2},
    {"molten glass", MaterialState::LIQUID, 2000, MaterialId::GLASS, 20, {MakeColor(237, 154, 59), MakeColor(255, 174, 33)}, 2},
    {"stoppium", MaterialState::STATIC, -1, MaterialId::NONE, 0, {MakeColor(194, 27, 209), MakeColor(147, 27, 171)}, 2},
    {"voidium", MaterialState::STATIC, -1, MaterialId::NONE, 0, {MakeColor(33, 7, 71), MakeColor(45, 7, 71)}, 2, {Reaction{MaterialId::ANY, MaterialId::VOIDIUM, MaterialId::AIR, 1000}}, 1}
}};

inline const MaterialDef& GetMaterialDef(MaterialId material) {
    return materials[static_cast<size_t>(material)];
}
