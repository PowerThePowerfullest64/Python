#pragma once

#include <SDL3/SDL.h>
#include <array>
#include <iostream>

#include "utils/math/vec2.hpp"
#include "utils/graphics/renderer.hpp"

constexpr auto LMB = SDL_BUTTON_LMASK;
constexpr auto RMB = SDL_BUTTON_RMASK;
constexpr auto MMB = SDL_BUTTON_MMASK;

class Input {
public:
    Input() = delete;

    // Checks and updates inputs for the current frame.
    static void Update(float _scroll) {
        previous = current;
        previousMouse = mouse;

        scroll = _scroll;

        // Key Input.
        const bool* state = SDL_GetKeyboardState(nullptr);

        for (int i = 0; i < SDL_SCANCODE_COUNT; ++i) {
            current[i] = state[i];
        }

        // Mouse Input and position.
        float x, y;
        mouse = SDL_GetMouseState(&x, &y);
        mousePosition =  {x, y};
    }

    // Pretty self-explanatory my guy.
    static bool KeyDown(SDL_Scancode key) {
        return current[key];
    }

    // Pretty self-explanatory my guy.
    static bool KeyPressed(SDL_Scancode key) {
        return current[key] && !(previous[key]);
    }

    // Pretty self-explanatory my guy.
    static bool KeyReleased(SDL_Scancode key) {
        return !(current[key]) && previous[key];
    }

    static bool MouseDown(SDL_MouseButtonFlags button) {
        // Bitmask type shit.
        return mouse & button;
    }

    static bool MousePressed(SDL_MouseButtonFlags button) {
        // Bitmask type shit x2.
        return (mouse & button) && !(previousMouse & button);
    }

    static bool MouseReleased(SDL_MouseButtonFlags button) {
        // Bitmask type shit x3.
        return !(mouse & button) && (previousMouse & button);
    }

    static bool ScrolledUp() {
        return scroll > 0.f;
    }

    static bool ScrolledDown() {
        return scroll < 0.f;
    }

    // Get mouse XY position as a Vec2<float>.
    static Vec2<float> MousePosition() {
        return mousePosition;
    }

    static Vec2<float> CameraMousePosition() {
        return mousePosition + Renderer::camera.pos - Renderer::camera.offset;
    }

private:
    static inline std::array<bool, SDL_SCANCODE_COUNT> current{};
    static inline std::array<bool, SDL_SCANCODE_COUNT> previous{};
    static inline float scroll = 0.f;

    static inline SDL_MouseButtonFlags mouse = 0;
    static inline SDL_MouseButtonFlags previousMouse = 0;

    static inline Vec2<float> mousePosition;
};