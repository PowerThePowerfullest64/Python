#pragma once

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

    void Place(int x, int y, int radius, Material material);

private:
    Grid<Material, 256, 256> grid;
    SDL_Renderer* sdl_renderer;
    SDL_Texture* tex;
};
