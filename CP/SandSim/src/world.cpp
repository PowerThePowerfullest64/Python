#include "world.hpp"

#include <iostream>

#include "utils/graphics/renderer.hpp"
#include "utils/math/vec2.hpp"
#include "utils/math/random.hpp"
#include "utils/logic/engine.hpp"

World::World(int blockSize) {
    BLOCK_SIZE = blockSize;
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
    for (int y = 0; y < grid.HEIGHT; y++) {
        for (int x = 0; x < grid.WIDTH; x++) {
            if (grid(x, y) == Material::AIR)
                continue;

            const Rectangle<int> rect(
                x * BLOCK_SIZE,
                y * BLOCK_SIZE,
                BLOCK_SIZE,
                BLOCK_SIZE
            );

            Renderer::DrawShape(rect, Colors::YELLOW, true);
        }
    }
}

void World::Place(int cx, int cy, int radius, Material material) {
    const int r2 = radius * radius;

    for (int dy = -radius; dy <= radius; dy++) {
        for (int dx = -radius; dx <= radius; dx++) {
            if (dx * dx + dy * dy <= r2) {
                const Vec2<int> pos(cx + dx, cy + dy);

                if (!grid.Within(pos))
                    return;

                grid(pos) = material;
            }
        }
    }
}
