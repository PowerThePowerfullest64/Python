#include <iostream>

#include "utils/graphics/renderer.hpp"
#include "utils/logic/engine.hpp"

#include "world.hpp"

int main() {
    Renderer::Init(1024, 1024, "Falling Sand Sim");
    Engine::SetFrameRate(120);

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

        Renderer::EndFrame();

        Engine::EndFrame();
    }
    

    Renderer::Deinit();
}
