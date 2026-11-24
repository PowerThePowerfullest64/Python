#pragma once

#include <cstdint>

#include <SDL3/SDL.h>
#include "utils/structures/grid.hpp"
#include "utils/math/random.hpp"
#include "utils/logic/engine.hpp"

#include "material.hpp"

class World {
public:
    static constexpr int BLOCK_SIZE = 2;
    int blockCount = 0;

    World();

    void Update();
    void Render() const;

    void Place(int x, int y, int radius, MaterialId material);

private:
    // Maxiumum amount of blocks a liquid can move horizontally in one update.
    static constexpr int liquidXMove = 25;
    // Maxiumum amount of blocks a gas can move horizontally in one update.
    static constexpr int gasMoveX = 10;
    static constexpr int WIDTH = 512, HEIGHT = 512;
    Grid<MaterialId, WIDTH, HEIGHT> materialIds;
    Grid<uint8_t, WIDTH, HEIGHT> paletteIndices;
    Grid<int16_t, WIDTH, HEIGHT> lifetimes;

    SDL_Renderer* sdl_renderer;
    SDL_Texture* tex;

    inline void Swap(const Vec2<int>& a, const Vec2<int>& b) {
        // Swap materials
        auto tmpMatId = materialIds(a);
        materialIds(a) = materialIds(b);
        materialIds(b) = tmpMatId;

        // Swap palettes
        auto tmpPaletteIndex = paletteIndices(a);
        paletteIndices(a) = paletteIndices(b);
        paletteIndices(b) = tmpPaletteIndex;

        // Swap lifetimes
        auto tmpLifetime = lifetimes(a);
        lifetimes(a) = lifetimes(b);
        lifetimes(b) = tmpLifetime;
    }

    constexpr inline bool CanMove(const MaterialDef& matDef, const Vec2<int>& to) {
        if (!materialIds.Within(to)) {
            return false;
        }

        MaterialDef otherMat = GetMaterialDef(materialIds(to));

        // If density allows the move to occur. Different for gases than other material states.
        bool densityAllows = matDef.state == MaterialState::GAS ? (matDef.density < otherMat.density) : (matDef.density > otherMat.density);

        return densityAllows && otherMat.state != MaterialState::STATIC;
    }

    constexpr inline bool TryHorizontalSpread(const Vec2<int>& position, const MaterialDef& matDef, bool leftFirst) {
        int firstDir = leftFirst ? -1 : 1;
        int secondDir = leftFirst ? 1 : -1;
        Vec2<int> offsetPos = {0, 0};
        
        if (CanMove(matDef, Vec2{position.x+firstDir, position.y})) {
            int targetXMove = Random::RangeI32(0, gasMoveX-1);
            while (CanMove(matDef, position+Vec2{offsetPos.x+firstDir, offsetPos.y}) && std::abs(offsetPos.x) < targetXMove) {
                offsetPos.x += firstDir;
            }
        } else if (CanMove(matDef, Vec2{position.x+secondDir, position.y})) {
            int targetXMove = Random::RangeI32(0, gasMoveX-1);
            while (CanMove(matDef, position+Vec2{offsetPos.x+secondDir, offsetPos.y}) && std::abs(offsetPos.x) < targetXMove) {
                offsetPos.x += secondDir;
            }
        } else {
            return false;
        }
        
        Swap(position, position+offsetPos);
        return true;
    }

    void UpdateLiquid(const Vec2<int>& position, const MaterialDef& matDef, uint32_t& rng);
    void UpdateSolid(const Vec2<int>& position, const MaterialDef& matDef, uint32_t& rng);
    void UpdateGas(const Vec2<int>& position, const MaterialDef& matDef, uint32_t& rng);
};
