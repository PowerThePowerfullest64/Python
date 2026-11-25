#include <iostream>

#include "utils/graphics/renderer.hpp"
#include "utils/logic/engine.hpp"

#include "particle_manager.hpp"

int main() {
    std::cout << "Particle sim starting...\n";
    
    Renderer::Init(1600, 900, "Particle Simulation", {512.f, 512.f});
    //DONT CHANGE FPS WILL FLICKER UNDER 500!!!😠😠😠
    Engine::SetFrameRate(120);

    double cameraSpeed = 400.0;
    double cameraShiftSpeed = 1000.0;
    float zoomSpeed = 1.f;
    ParticleManager pm;

    // If it's running
    bool running = true;
    while (running) {
        Engine::BeginFrame();
        Engine::PollEvents(running);

        double dt = Engine::GetDeltaTime();

        Renderer::BeginFrame();
        Renderer::Clear(Color(30, 0, 70));

        if (Input::MouseDown(LMB) || Input::MousePressed(RMB)) {
            Vec2<float> mp = Input::CameraMousePosition();
            Vec2<double> mousePos = Vec2<double>(mp.x, mp.y);
            pm.AddParticle(mousePos.x, mousePos.y);
        }

        double realCamSpeed = (Input::KeyDown(SDL_SCANCODE_LSHIFT) ? cameraShiftSpeed : cameraSpeed) / Renderer::camera.zoom * dt;
        if (Input::KeyDown(SDL_SCANCODE_W)) {
            Renderer::camera.pos.y -= realCamSpeed;
        } if (Input::KeyDown(SDL_SCANCODE_S)) {
            Renderer::camera.pos.y += realCamSpeed;
        } if (Input::KeyDown(SDL_SCANCODE_A)) {
            Renderer::camera.pos.x -= realCamSpeed;
        } if (Input::KeyDown(SDL_SCANCODE_D)) {
            Renderer::camera.pos.x += realCamSpeed;
        }
        
        if (Input::ScrolledUp()) {
            Renderer::camera.zoom *= 1.f + (0.1f * zoomSpeed);
        } else if (Input::ScrolledDown()) {
            Renderer::camera.zoom /= 1.f + (0.1f * zoomSpeed);
        }

        if (Input::KeyDown(SDL_SCANCODE_DELETE))
            pm.Clear();
        
        pm.Update();
        pm.Draw();
        
        Renderer::DrawText(Vec2(4, 4), "FPS: " + std::to_string(Engine::GetFPS()), Colors::WHITE, true);
        Renderer::DrawText(Vec2(4, 30), "Particle count: " + std::to_string(pm.AmountOfParticles()), Colors::WHITE, true);

        Renderer::EndFrame();

        Engine::EndFrame();
    }
}

