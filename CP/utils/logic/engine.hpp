#pragma once

#include <chrono>
#include <thread>
#include <cmath>

#include <SDL3/SDL.h>

#include "utils/logic/input.hpp"

using namespace std::chrono_literals;
using Clock = std::chrono::steady_clock;
using Seconds = std::chrono::duration<double>;

class Engine {
public:
    // Updates input and events.
    static inline bool PollEvents() {
        float scroll = 0.f;
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            // Give events to ImGui also!
            ImGui_ImplSDL3_ProcessEvent(&event);

            switch (event.type)
            {
            case SDL_EVENT_QUIT:
                return false;
            case SDL_EVENT_KEY_DOWN:
                if (event.key.scancode == SDL_SCANCODE_ESCAPE)
                    return false;
                break;
            case SDL_EVENT_MOUSE_WHEEL:
                scroll = event.wheel.y;
                break;
            default:
                break;
            }
        }

        Input::Update(scroll);

        return true;
    }

    // Should be at start of game loop. Calculates delta time as well as other frame information.
    static inline void BeginFrame() {
        frameStart = Clock::now();
        if (last != Clock::time_point{})
            dt = Seconds(frameStart - last).count();
        last = frameStart;
    }

    // Should be at end of game loop. Calculates and waits the necessary amount of time to keep the target frame rate.
    static inline void EndFrame() {
        auto target = frameStart + std::chrono::duration_cast<Clock::duration>(targetFrameDuration);
        auto remaining = target - Clock::now();
        if (remaining > std::chrono::milliseconds(2))
            std::this_thread::sleep_for(remaining - std::chrono::milliseconds(1));
        while (Clock::now() < target) {}
    }

    // Get duration of the last frame.
    static inline double GetDeltaTime() {
        return dt;
    }

    // Get the frame rate of the window.
    static inline int GetFPS() {
        return std::lround(1.0 / dt);
    }

    // Sets the framerate to a specified value. Setting it to 0 uncaps the framerate.
    static inline void SetFrameRate(int target) {
        if (target <= 0) {
            targetFrameDuration = 0s;
            return;
        }

        targetFrameDuration = 1.0s / target;
    }

private:
    static inline Clock::time_point last{};
    static inline Clock::time_point frameStart{};
    static inline double dt = 0.1;
    static inline Seconds targetFrameDuration{1.0 / 60.0};
};