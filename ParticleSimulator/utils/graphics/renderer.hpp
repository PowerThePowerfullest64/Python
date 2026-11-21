#pragma once

#include <SDL3/SDL.h>
#include <iostream>

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

#include "utils/math/vec2.hpp"
#include "utils/structures/shapes.hpp"
#include "utils/graphics/color.hpp"
#include "utils/graphics/texture_manager.hpp"

class Camera2D {
public:
    Vec2<float> pos;
    Vec2<float> offset;
    float zoom = 1.f;
};

inline SDL_Window* CreateWindow(int width, int height, const std::string& name);
inline SDL_Renderer* CreateRenderer(SDL_Window* window);

class Renderer {
public:
    static inline Camera2D camera;

    // Translates a position into camera-space.
    template<typename T>
    static Vec2<T> ToCamera(const Vec2<T>& pos) {
        return {(pos - camera.pos) * camera.zoom + camera.offset};
    }

    Renderer() = delete;

    // Initializes the renderer and all its sub-modules.
    static void Init(int width, int height, const std::string& name, const Vec2<float>& camOffset = {0.f, 0.f}) {
        SDL_Init(SDL_INIT_VIDEO);
        sdl_window = CreateWindow(width, height, name);
        sdl_renderer = CreateRenderer(sdl_window);
        TM::Init(sdl_renderer, "utils/textures/");
        TM::LoadTextures();

        camera.offset = camOffset;

        // INIT IMGUI
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui::StyleColorsDark();
        ImGui_ImplSDL3_InitForSDLRenderer(sdl_window, sdl_renderer);
        ImGui_ImplSDLRenderer3_Init(sdl_renderer);

        std::cout << "Initialized Renderer!\n";
    }

    // Deinitializes the renderer and all its sub-modules.
    static void Deinit() {
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();

        TM::UnloadTextures();

        SDL_DestroyRenderer(sdl_renderer);
        SDL_DestroyWindow(sdl_window);
        SDL_Quit();

        std::cout << "Deinitialized Renderer!\n";
    }

    template<typename T>
    static void DrawText(const Vec2<T>& position, const std::string& text, const Color& color = Colors::WHITE, bool cameraRelative = false) {
        Vec2<T> usedPosition = cameraRelative ? position : ToCamera(position);

        SetColor(color);
        SDL_RenderDebugText(sdl_renderer, static_cast<float>(usedPosition.x), static_cast<float>(usedPosition.y), text.c_str());
    }

    // Render Line.
    template<typename T>
    static void DrawShape(const Line<T>& line, const Color& color = Colors::WHITE, bool cameraRelative = false) {
        // Line coords in camera-space.
        Line<T> usedLine = cameraRelative ? line : Line{ToCamera(line.from), ToCamera(line.to)};

        SetColor(color);
        SDL_RenderLine(sdl_renderer, usedLine.from.x, usedLine.from.y, usedLine.to.x, usedLine.to.y);
    }
    // Render Rectangle.
    template<typename T>
    static void DrawShape(const Rectangle<T>& rectangle, const Color& color = Colors::WHITE, bool filled = true, bool cameraRelative = false) {
        SetColor(color);
        SDL_FRect sdlRect = rectangle;

        if (!cameraRelative) {
            // Translate to camera-space.
            Vec2<T> camRectPosition = ToCamera(rectangle.pos);
            sdlRect.x = static_cast<float>(camRectPosition.x);
            sdlRect.y = static_cast<float>(camRectPosition.y);
            sdlRect.w = static_cast<float>(rectangle.size.x * camera.zoom);
            sdlRect.h = static_cast<float>(rectangle.size.y * camera.zoom);
        }

        if (filled) SDL_RenderFillRect(sdl_renderer, &sdlRect);
        else SDL_RenderRect(sdl_renderer, &sdlRect);
    }
    // Render Circle.
    template<typename T>
    static void DrawShape(const Circle<T>& circle, const Color& color = Colors::WHITE, bool filled = true, bool cameraRelative = false) {
        Circle<T> usedCircle = cameraRelative ? circle : Circle{ToCamera(circle.pos), static_cast<T>(circle.radius * camera.zoom)};

        SetColor(color);
        if (filled) DrawCircleFilled(usedCircle);
        else DrawCircle(usedCircle);
    }

    template<typename T>
    static void DrawTexture(SDL_Texture* texture, const Rectangle<T>& _src, const Rectangle<T>& _dst) {
        SDL_FRect src = _src;
        SDL_FRect dst = _dst;

        // Translate to camera-space.
        Vec2<float> camTexturePos = ToCamera(Vec2<float>{dst.x, dst.y});
        dst.x = camTexturePos.x;
        dst.y = camTexturePos.y;
        dst.w = static_cast<float>(_dst.size.x * camera.zoom);
        dst.h = static_cast<float>(_dst.size.y * camera.zoom);

        SDL_RenderTexture(sdl_renderer, texture, &src, &dst);
    }

    static void DrawTexture(SDL_Texture* texture, const Vec2<float>& pos, const Vec2<float>& scale = {1, 1}) {
        SDL_FRect src = { 0.f, 0.f,
            static_cast<float>(texture->w),
            static_cast<float>(texture->h)
        };

        SDL_FRect dst;

        // Translate to camera-space.
        Vec2<float> camTexturePos = ToCamera(Vec2<float>{pos.x, pos.y});
        dst.x = camTexturePos.x;
        dst.y = camTexturePos.y;
        dst.w = static_cast<float>(texture->w) * scale.x * camera.zoom;
        dst.h = static_cast<float>(texture->h) * scale.y * camera.zoom;

        SDL_RenderTexture(sdl_renderer, texture, &src, &dst);
    }

    static void DrawTexture(SDL_Texture* texture) {
        Rectangle<float> src = {0, 0, static_cast<float>(texture->w), static_cast<float>(texture->h)};
        Rectangle<float> dst = {0, 0, static_cast<float>(texture->w) * camera.zoom, static_cast<float>(texture->h) * camera.zoom};

        // Translate to camera-space.
        dst.pos = ToCamera(dst.pos);

        DrawTexture(texture, src, dst);
    }

    // Set the draw color of SDL.
    static inline void SetColor(const Color& color) {
        SDL_SetRenderDrawColor(sdl_renderer, color.r, color.g, color.b, color.a);
    }

    // Fill the window with a specified color.
    static void Clear(const Color& color) {
        SetColor(color);
        SDL_RenderClear(sdl_renderer);
    }

    // Call this before you render things.
    static inline void BeginFrame() {
        // IMGUI NEW FRAME
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
    }

    // Call this after you render things.
    static inline void EndFrame() {
        // RENDER IMGUI DATA TO SCREEN
        ImGui::Render();
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), sdl_renderer);

        SDL_RenderPresent(sdl_renderer);
    }

private:
    static inline SDL_Renderer* sdl_renderer;
    static inline SDL_Window* sdl_window;
    static inline float deltaTime = 0.1f;

    template<typename T>
    static inline void DrawCircle(const Circle<T>& circle) {
        int radius = static_cast<int>(circle.radius);
        int x0 = static_cast<int>(circle.pos.x);
        int y0 = static_cast<int>(circle.pos.y);

        int x = radius - 1;
        int y = 0;
        int dx = 1;
        int dy = 1;
        int err = dx - (radius << 1);

        while (x >= y) {
            SDL_RenderPoint(sdl_renderer, x0 + x, y0 + y);
            SDL_RenderPoint(sdl_renderer, x0 + y, y0 + x);
            SDL_RenderPoint(sdl_renderer, x0 - y, y0 + x);
            SDL_RenderPoint(sdl_renderer, x0 - x, y0 + y);
            SDL_RenderPoint(sdl_renderer, x0 - x, y0 - y);
            SDL_RenderPoint(sdl_renderer, x0 - y, y0 - x);
            SDL_RenderPoint(sdl_renderer, x0 + y, y0 - x);
            SDL_RenderPoint(sdl_renderer, x0 + x, y0 - y);

            if (err <= 0) {
                y++;
                err += dy;
                dy += 2;
            }

            if (err > 0) {
                x--;
                dx += 2;
                err += dx - (radius << 1);
            }
        }
    }

    template<typename T>
    static inline void DrawCircleFilled(const Circle<T>& circle) {
        int x = static_cast<int>(circle.pos.x);
        int y = static_cast<int>(circle.pos.y);
        int radius = static_cast<int>(circle.radius);

        for (int dy = -radius; dy <= radius; ++dy) {
            int dx = static_cast<int>(std::sqrtf(radius * radius - dy * dy));
            SDL_RenderLine(
                sdl_renderer,
                x - dx, y + dy,
                x + dx, y + dy
            );
        }
    }
};

inline SDL_Window* CreateWindow(int width, int height, const std::string& name) {
    SDL_Window* window = SDL_CreateWindow(name.c_str(), width, height, 0);

    if (!window) {
        std::cout << "Window failed: " << SDL_GetError() << "\n";
        SDL_Quit();
    }

    return window;
}

inline SDL_Renderer* CreateRenderer(SDL_Window* window) {
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

    if (!renderer) {
        std::cout << "Renderer failed: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    return renderer;
}