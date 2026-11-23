#include "world.hpp"

#include <iostream>

#include "utils/graphics/renderer.hpp"
#include "utils/math/vec2.hpp"
#include "utils/math/random.hpp"
#include "utils/logic/engine.hpp"

World::World(int blockSize) {
    BLOCK_SIZE = blockSize;

    sdl_renderer = Renderer::GetSDLRenderer();
    tex = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING, 256, 256);
    SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);
}

void World::Update() {
    const auto frameCount = Engine::GetFrameCount();
    for (int y = grid.HEIGHT-1; y >= 0; y--) {
        const bool scanLeft = ((y + frameCount) & 1) == 0;

        for (int i = 0; i < grid.WIDTH; i++) {
            int x = scanLeft ? i : grid.WIDTH - 1 - i;

            const Material type = grid(x, y);
            if (type == Material::AIR)
                continue;

            const Vec2<int> d = {x, y+1};
            const Vec2<int> dr = {x+1, y+1};
            const Vec2<int> dl = {x-1, y+1};

            const bool leftFirst = Random::RangeI32(0, 1) == 0;
            const Vec2<int> firstDiag = leftFirst ? dl : dr;
            const Vec2<int> secondDiag = leftFirst ? dr : dl;

            if (grid.Within(d) && grid(d) == Material::AIR) {
                grid(Vec2(x, y)) = Material::AIR;
                grid(d) = type;
            } else if (grid.Within(firstDiag) && grid(firstDiag) == Material::AIR) {
                grid(Vec2(x, y)) = Material::AIR;
                grid(firstDiag) = type;
            } else if (grid.Within(secondDiag) && grid(secondDiag) == Material::AIR) {
                grid(Vec2(x, y)) = Material::AIR;
                grid(secondDiag) = type;
            }
        }
    }
}

void World::Render() const {
    void* pixels;
    int pitch;
    SDL_LockTexture(tex, nullptr, &pixels, &pitch);

    for (int y = 0; y < grid.HEIGHT; y++) {
        uint32_t* row = (uint32_t*)((uint8_t*)pixels + y * pitch);
        for (int x = 0; x < grid.WIDTH; x++) {
            row[x] = grid(x, y) == Material::AIR ? 0xFFFA0000 : 0xFF00FFFF;
        }
    }
    SDL_UnlockTexture(tex);

    SDL_FRect dst = {0, 0, static_cast<float>(grid.WIDTH * BLOCK_SIZE), static_cast<float>(grid.HEIGHT * BLOCK_SIZE)};
    SDL_RenderTexture(sdl_renderer, tex, nullptr, &dst);
}

void World::Place(int cx, int cy, int radius, Material material) {
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            Vec2<int> pos = Vec2(cx+x, cy+y);
            if (!grid.Within(pos))
                continue;

            Material mat = grid(pos);
            if (mat == material)
                continue;

            if (material == Material::AIR)
                blockCount--;
            else
                blockCount++;


            grid(pos) = material;
        }
    }
}
