#include <iostream>
#include <string>

#include "utils/graphics/renderer.hpp"
#include "utils/logic/engine.hpp"

#include "world.hpp"

int main() {
    Renderer::Init(1024, 1024, "Falling Sand Sim");
    Engine::SetFrameRate(0);

    World world(4);

    world.Place(128, 128, 8, Material::SAND);

    bool running = true;
    while (running) {
        Engine::BeginFrame();
        Engine::PollEvents(running);

        if (Input::MouseDown(LMB) || Input::MouseDown(RMB)) {
            const Vec2<float> mousePos = Input::MousePosition();
            const Vec2<int> cellPos = Vec2<int>(mousePos / world.BLOCK_SIZE);

            world.Place(cellPos.x, cellPos.y, 8, (Input::MouseDown(LMB) ? Material::SAND : Material::AIR));
        }

        world.Update();

        Renderer::BeginFrame();
        Renderer::Clear(Colors::BLUE);

        world.Render();

        Renderer::DrawText(Vec2(4, 4), "FPS: " + std::to_string(Engine::GetFPS()), Colors::WHITE, true);
        Renderer::DrawText(Vec2(4, 12), "Block count: " + std::to_string(world.blockCount), Colors::WHITE, true);

        Renderer::EndFrame();

        Engine::EndFrame();
    }
    

    Renderer::Deinit();
}
