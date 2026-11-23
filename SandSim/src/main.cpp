#include <iostream>
#include <string>

#include "utils/graphics/renderer.hpp"
#include "utils/logic/engine.hpp"

#include "world.hpp"

int main() {
    Renderer::Init(1024, 1024, "Falling Sand Sim");
    Engine::SetFrameRate(120);

    bool paused = false;
    World world(4);
    world.Place(128, 128, 8, MaterialId::SAND);

    bool running = true;
    while (running) {
        Engine::BeginFrame();
        Engine::PollEvents(running);

        if (Input::MouseDown(LMB) || Input::MouseDown(RMB)) {
            const Vec2<float> mousePos = Input::MousePosition();
            const Vec2<int> cellPos = Vec2<int>(mousePos / world.BLOCK_SIZE);

            world.Place(cellPos.x, cellPos.y, 8, (Input::MouseDown(LMB) ? MaterialId::SAND : MaterialId::AIR));
        }

        if (Input::KeyPressed(SDL_SCANCODE_SPACE))
            paused = !paused;

        if (!paused)
            world.Update();

        Renderer::BeginFrame();
        Renderer::Clear(Colors::BLUE);

        world.Render();

        Renderer::DrawText(Vec2(4, 4), "FPS: " + std::to_string(Engine::GetFPS()), Colors::WHITE, true);
        Renderer::DrawText(Vec2(4, 16), "Block count: " + std::to_string(world.blockCount), Colors::WHITE, true);
        if (paused) { Renderer::DrawText(Vec2(4, 28), "Simulation paused", Colors::WHITE, true); }

        Renderer::EndFrame();

        Engine::EndFrame();
    }
    

    Renderer::Deinit();
}
