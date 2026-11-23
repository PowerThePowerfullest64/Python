#pragma once

#include <cstdint>

#include "utils/structures/grid.hpp"
#include <SDL3/SDL.h>

#include "material.hpp"

class World {
public:
    int BLOCK_SIZE;
    int blockCount = 0;

    World(int blockSize);

    void Update();
    void Render() const;

    void Place(int x, int y, int radius, MaterialId material);

private:
    static constexpr int WIDTH = 256, HEIGHT = 256;
    Grid<MaterialId, WIDTH, HEIGHT> materialIds;
    Grid<uint8_t, WIDTH, HEIGHT> paletteIndices;

    SDL_Renderer* sdl_renderer;
    SDL_Texture* tex;
};
