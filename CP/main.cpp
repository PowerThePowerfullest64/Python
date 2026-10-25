#include <iostream>

#include "utils/graphics/renderer.hpp"
#include "utils/logic/engine.hpp"
#include "utils/math/random.hpp"

int main() {
    Renderer::Init(1024, 1024, "This is a window!");
    Engine::SetFrameRate(60);

    bool running = true;

    while (running) {
        Engine::BeginFrame();
        running = Engine::PollEvents();

        Renderer::BeginFrame();

        Renderer::Clear(Colors::LIGHT_BLUE);
        Renderer::DrawText(Vec2{16, 16}, "FPS: " + std::to_string(Engine::GetFPS()), Colors::GREEN, true);

        Renderer::EndFrame();

        Engine::EndFrame();
    }

    Renderer::Deinit();

    return 0;
}