#include "world.hpp"

#include <iostream>
#include <array>

#include "utils/graphics/renderer.hpp"
#include "utils/math/vec2.hpp"
#include "utils/math/random.hpp"
#include "utils/logic/engine.hpp"

World::World() {
    materialIds.Fill(MaterialId::AIR);
    paletteIndices.Fill(0);
    lifetimes.Fill(-1);

    sdl_renderer = Renderer::GetSDLRenderer();
    tex = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
    SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);
}

void World::Update() {
    const auto frameCount = Engine::GetFrameCount();
    // Seeded rng every frame.
    uint32_t rng = static_cast<uint32_t>(frameCount * 747796405u + 2891336453u);
    const uint8_t tick = (frameCount & 1) + 1;

    for (int y = HEIGHT-1; y >= 0; y--) {
        const bool scanLeft = ((y + frameCount) & 1) == 0;

        for (int i = 0; i < WIDTH; i++) {
            int x = scanLeft ? i : WIDTH - 1 - i;

            const MaterialDef matDef = GetMaterialDef(materialIds(x, y));
            if (matDef.state == MaterialState::LIQUID)
                UpdateLiquid(Vec2(x, y), matDef, rng);
            else if (matDef.state == MaterialState::SOLID)
                UpdateSolid(Vec2(x, y), matDef, rng);
            else if (matDef.state == MaterialState::STATIC) {
                if (matDef.lifetime > 0 || matDef.reactionsSize > 0) // oOnly update a static if it has active properties.
                    UpdateStatic(Vec2(x, y), matDef, rng);
            }
        }
    }

    for (int y = 0; y < HEIGHT; y++) {
        const bool scanLeft = ((y + frameCount) & 1) == 0;

        for (int i = 0; i < WIDTH; i++) {
            int x = scanLeft ? i : WIDTH - 1 - i;

            const MaterialDef matDef = GetMaterialDef(materialIds(x, y));
            if (matDef.state == MaterialState::GAS)
                UpdateGas(Vec2(x, y), matDef, rng);
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

void World::Place(int cx, int cy, int radius, MaterialId material, bool force) {
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            Vec2<int> pos = Vec2(cx+x, cy+y);
            if (!materialIds.Within(pos))
                continue;

            MaterialId mat = materialIds(pos);
            if (mat == material)
                continue;
            if (mat != MaterialId::AIR && !force)
                continue;

            if (material == MaterialId::AIR)
                blockCount--;
            else if (mat == MaterialId::AIR)
                blockCount++;

            materialIds(pos) = material;
            // Choose random color for material.
            const MaterialDef& matDef = GetMaterialDef(material);
            paletteIndices(pos) = Random::RangeUI32(0, matDef.paletteSize-1);
            lifetimes(pos) = Random::RangeUI32(matDef.lifetime/6, matDef.lifetime);
        }
    }
}

void World::UpdateLiquid(const Vec2<int>& position, const MaterialDef& matDef, uint32_t& rng) {
    if (matDef.lifetime > 0) {
        lifetimes(position)--;
        if (lifetimes(position) == 0) {
            Place(position.x, position.y, 0, matDef.product);
            return;
        }
    }

    if (TryReact(position, matDef, rng)) {
        return;
    }

    Vec2<int> d = {position.x, position.y+1};

    if (CanMove(matDef, d)) {
        Swap(position, d);
    } else {
        rng ^= rng << 13;
        rng ^= rng >> 17;
        rng ^= rng << 5;
        const bool leftFirst = (rng & 1) == 0;

        const Vec2<int> dr = {position.x+1, position.y+1};
        const Vec2<int> dl = {position.x-1, position.y+1};

        const Vec2<int> firstDiagonal = leftFirst ? dl : dr;
        const Vec2<int> secondDiagonal = leftFirst ? dr : dl;

        if (CanMove(matDef, firstDiagonal))
            Swap(position, firstDiagonal);
        else if (CanMove(matDef, secondDiagonal))
            Swap(position, secondDiagonal);
        else {
            TryHorizontalSpread(position, matDef, leftFirst);
        }
    }
}

void World::UpdateSolid(const Vec2<int>& position, const MaterialDef& matDef, uint32_t& rng) {
    if (matDef.lifetime > 0) {
        lifetimes(position)--;
        if (lifetimes(position) == 0) {
            Place(position.x, position.y, 0, matDef.product);
            return;
        }
    }

    if (TryReact(position, matDef, rng)) {
        return;
    }

    Vec2<int> d = {position.x, position.y+1};

    if (CanMove(matDef, d)) {
        Swap(position, d);
    } else {
        rng ^= rng << 13;
        rng ^= rng >> 17;
        rng ^= rng << 5;
        const bool leftFirst = (rng & 1) == 0;

        Vec2<int> dr = {position.x+1, position.y+1};
        Vec2<int> dl = {position.x-1, position.y+1};

        Vec2<int> firstDiagonal = leftFirst ? dl : dr;
        Vec2<int> secondDiagonal = leftFirst ? dr : dl;

        if (CanMove(matDef, firstDiagonal))
            Swap(position, firstDiagonal);
        else if (CanMove(matDef, secondDiagonal))
            Swap(position, secondDiagonal);
    }
}

void World::UpdateGas(const Vec2<int>& position, const MaterialDef& matDef, uint32_t& rng) {
    if (matDef.lifetime > 0) {
        lifetimes(position)--;
        if (lifetimes(position) == 0) {
            Place(position.x, position.y, 0, matDef.product);
            return;
        }
    }

    if (TryReact(position, matDef, rng)) {
        return;
    }

    rng ^= rng << 13;
    rng ^= rng >> 17;
    rng ^= rng << 5;
    
    const bool leftFirst = (rng & 1) == 0;
    const uint32_t roll = (rng >> 1) % 100;
    
    const Vec2<int> u = {position.x, position.y-1};
    const Vec2<int> ur = {position.x+1, position.y-1};
    const Vec2<int> ul = {position.x-1, position.y-1};

    const Vec2<int> firstDiag = leftFirst ? ul : ur;
    const Vec2<int> secondDiag = leftFirst ? ur : ul;

    if (roll < 15) {
        // If it spread horizontally then we are done.
        if (TryHorizontalSpread(position, matDef, leftFirst))
            return;
    }

    std::array<Vec2<int>, 3> moves = roll < 60
        ? std::array<Vec2<int>, 3>{u, firstDiag, secondDiag}
        : std::array<Vec2<int>, 3>{firstDiag, secondDiag, u};
    
    for (const auto& target : moves) {
        if (CanMove(matDef, target)) {
            Swap(position, target);
            return;
        }
    }

    TryHorizontalSpread(position, matDef, leftFirst);
}

void World::UpdateStatic(const Vec2<int>& position, const MaterialDef& matDef, uint32_t& rng) {
    if (matDef.lifetime > 0) {
        lifetimes(position)--;
        if (lifetimes(position) == 0) {
            Place(position.x, position.y, 0, matDef.product);
            return;
        }
    }

    TryReact(position, matDef, rng);
}

bool World::TryReact(const Vec2<int>& position, const MaterialDef& matDef, uint32_t& rng) {
    if (matDef.reactionsSize == 0)
        return false;
    
    MaterialId matId = materialIds(position);
    
    static constexpr std::array<Vec2<int>, 4> dirs = {
        Vec2{0, -1}, Vec2{0, 1}, Vec2{-1, 0}, Vec2{1, 0}
    };

    for (const auto& dir : dirs) {
        Vec2<int> nPos = position + dir;
        if (!materialIds.Within(nPos))
            continue;
        
        MaterialId nId = materialIds(nPos);
        for (int i = 0; i < matDef.reactionsSize; i++) {
            const auto& rule = matDef.reactions[i];
            if (rule.neighbor != nId && (rule.neighbor != MaterialId::ANY || nId == MaterialId::AIR || matId == nId))
                continue;
            
            rng ^= rng << 13;
            rng ^= rng >> 17;
            rng ^= rng << 5;

            if ((rng % 1000) < rule.chance) {
                if (rule.otherBecomes != nId) {
                    Place(nPos.x, nPos.y, 0, rule.otherBecomes);
                }
                if (rule.selfBecomes != matId) {
                    Place(position.x, position.y, 0, rule.selfBecomes);
                    return true;
                }
            }
        }
    }
    return false;
}

void World::Clear() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            Place(x, y, 0, MaterialId::AIR);
        }
    }
}
