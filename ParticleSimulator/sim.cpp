#include <iostream>

#include "utils/graphics/renderer.hpp"
#include "utils/logic/engine.hpp"

#include "particle_manager.hpp"

int main() {
    std::cout << "Particle sim starting...\n";
    
    Renderer::Init(1600, 900, "Particle Simulation");
    //DONT CHANGE FPS WILL FLICKER UNDER 500!!!😠😠😠
    Engine::SetFrameRate(120);

    ParticleManager pm;

    // If it's running
    bool running = true;
    while (running) {
        Engine::BeginFrame();
        Engine::PollEvents(running);

        Renderer::BeginFrame();
        Renderer::Clear(Color(30, 0, 70));

        if (Input::MouseDown(LMB)) {
            Vec2<float> mp = Input::MousePosition();
            Vec2<double> mousePos = Vec2<double>(mp.x, mp.y);
            pm.AddParticle(mousePos.x, mousePos.y);
        }

        if (Input::KeyDown(SDL_SCANCODE_DELETE))
            pm.Clear();
        
        pm.Update();
        pm.Draw();
        
        Renderer::DrawText(Vec2(4, 4), "FPS: " + std::to_string(Engine::GetFPS()));
        Renderer::DrawText(Vec2(4, 30), "Particle count: " + std::to_string(pm.AmountOfParticles()));

        Renderer::EndFrame();

        Engine::EndFrame();
    }
}

