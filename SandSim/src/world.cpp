#include "world.hpp"

#include <iostream>

#include "utils/graphics/renderer.hpp"
#include "utils/math/vec2.hpp"
#include "utils/math/random.hpp"
#include "utils/logic/engine.hpp"

World::World(int blockSize) {
    BLOCK_SIZE = blockSize;

    materialIds.Fill(MaterialId::AIR);
    paletteIndices.Fill(0);

    sdl_renderer = Renderer::GetSDLRenderer();
    tex = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING, 256, 256);
    SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);
}

void World::Update() {
    const auto frameCount = Engine::GetFrameCount();
    for (int y = HEIGHT-1; y >= 0; y--) {
        const bool scanLeft = ((y + frameCount) & 1) == 0;

        for (int i = 0; i < WIDTH; i++) {
            int x = scanLeft ? i : WIDTH - 1 - i;

            const MaterialId type = materialIds(x, y);
            if (type == MaterialId::AIR)
                continue;

            const Vec2<int> d = {x, y+1};
            const Vec2<int> dr = {x+1, y+1};
            const Vec2<int> dl = {x-1, y+1};

            const bool leftFirst = Random::RangeI32(0, 1) == 0;
            const Vec2<int> firstDiag = leftFirst ? dl : dr;
            const Vec2<int> secondDiag = leftFirst ? dr : dl;

            if (materialIds.Within(d) && materialIds(d) == MaterialId::AIR) {
                // Swap materials
                auto tmpId = materialIds(x, y);
                materialIds(x, y) = materialIds(d);
                materialIds(d) = tmpId;

                // Swap palettes
                auto tmpPaletteIndex = paletteIndices(x, y);
                paletteIndices(x, y) = paletteIndices(d);
                paletteIndices(d) = tmpPaletteIndex;
            } else if (materialIds.Within(firstDiag) && materialIds(firstDiag) == MaterialId::AIR) {
                // Swap materials
                auto tmpId = materialIds(x, y);
                materialIds(x, y) = materialIds(firstDiag);
                materialIds(firstDiag) = tmpId;

                // Swap palettes
                auto tmpPaletteIndex = paletteIndices(x, y);
                paletteIndices(x, y) = paletteIndices(firstDiag);
                paletteIndices(firstDiag) = tmpPaletteIndex;
            } else if (materialIds.Within(secondDiag) && materialIds(secondDiag) == MaterialId::AIR) {
                // Swap materials
                auto tmpId = materialIds(x, y);
                materialIds(x, y) = materialIds(secondDiag);
                materialIds(secondDiag) = tmpId;

                // Swap palettes
                auto tmpPaletteIndex = paletteIndices(x, y);
                paletteIndices(x, y) = paletteIndices(secondDiag);
                paletteIndices(secondDiag) = tmpPaletteIndex;
            }
        }
    }
}

void World::Render() const {
    void* pixels;
    int pitch;
    SDL_LockTexture(tex, nullptr, &pixels, &pitch);

    for (int y = 0; y < HEIGHT; y++) {
        uint32_t* row = (uint32_t*)((uint8_t*)pixels + y * pitch);
        for (int x = 0; x < WIDTH; x++) {
            MaterialId matId = materialIds(x, y);
            // Get color from palette index!
            row[x] = GetMaterialDef(matId).palette[paletteIndices(x, y)];
        }
    }
    SDL_UnlockTexture(tex);

    SDL_FRect dst = {0, 0, static_cast<float>(WIDTH * BLOCK_SIZE), static_cast<float>(HEIGHT * BLOCK_SIZE)};
    SDL_RenderTexture(sdl_renderer, tex, nullptr, &dst);
}

void World::Place(int cx, int cy, int radius, MaterialId material) {
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            Vec2<int> pos = Vec2(cx+x, cy+y);
            if (!materialIds.Within(pos))
                continue;

            MaterialId mat = materialIds(pos);
            if (mat == material)
                continue;

            if (material == MaterialId::AIR)
                blockCount--;
            else
                blockCount++;

            materialIds(pos) = material;
            // Choose random color for material.
            const MaterialDef& matDef = GetMaterialDef(material);
            paletteIndices(pos) = Random::RangeUI32(0, matDef.paletteSize-1);
        }
    }
}
