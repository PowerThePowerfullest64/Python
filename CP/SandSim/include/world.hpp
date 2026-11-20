#pragma once

#include <memory>

#include "utils/structures/grid.hpp"

#include "material.hpp"

class World {
public:
    int BLOCK_SIZE;

    World(int blockSize);

    void Update();
    void Render() const;

    void Place(int x, int y, int radius, Material material);

private:
    Grid<Material, 256, 256> grid;
};
