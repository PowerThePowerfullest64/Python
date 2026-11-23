#pragma once

#include <cstdint>

#include "utils/structures/grid.hpp"
#include <SDL3/SDL.h>

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
    static constexpr int WIDTH = 512, HEIGHT = 512;
    Grid<MaterialId, WIDTH, HEIGHT> materialIds;
    Grid<uint8_t, WIDTH, HEIGHT> paletteIndices;

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
    }
};
