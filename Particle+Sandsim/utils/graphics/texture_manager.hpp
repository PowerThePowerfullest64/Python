#pragma once

#include <SDL3/SDL.h>
#include <unordered_map>
#include <iostream>

class TextureManager {
public:
    // Don't instance ts! (YOU CANT MUAHAHAHA)
    TextureManager() = delete;

    // Initialize TextureManager and specify the path where you keep your textures.
    static void Init(SDL_Renderer* _renderer, const std::string& _texturesPath) {
        renderer = _renderer;
        texturesPath = _texturesPath;
        initialized = true;
    }

    // Get a pointer to a texture specified by the name. Note that the name should not include path or file extension, only the name of file itself.
    static SDL_Texture* Get(const std::string& name) {
        if (!initialized) {
            std::cout << "Warning: Initialize TextureManager with Init() before usage.\n";
            return textures.begin()->second;
        }

        auto it = textures.find(name);

        if (it == textures.end()) {
            std::cout << "Warning: Could not find texture with name '" << name << "'.\n";
            // Return the "NULL TEXTURE" texture.
            return textures.begin()->second;
        }

        return it->second;
    }

    // Loads all textures from folder into VRAM.
    static void LoadTextures() {
        if (!initialized) {
            std::cout << "Warning: Initialize TextureManager with Init() before usage.\n";
            return;
        }

        // Manual loading until I get myself to learn to iterate over a folder... So proud it works though!
        Add("null", true); // The "NULL TEXTURE" texture, used if a texture can't be found. Don't even think about what happens if this texture cannot be found.

        // Add your own textures here, after the null texture! :)
        // Example: Add("name_of_your_texture_without_extension_or_path");
        // Note: All textures should be of filetype .bmp!
    }

    // Unload all textures from VRAM.
    static void UnloadTextures() {
        if (!initialized) {
            std::cout << "Warning: Initialize TextureManager with Init() before usage.\n";
            return;
        }

        auto it = textures.begin();
        while (it != textures.end()) {
            SDL_DestroyTexture(it->second);

            textures.erase(it->first);
            it++;
        }
    }

private:
    static inline SDL_Renderer* renderer = nullptr;
    static inline bool initialized = false;
    static inline std::unordered_map<std::string, SDL_Texture*> textures = {};
    static inline std::string texturesPath = "";
    static inline std::string nullTexturePath = "utils/misc/";

    static inline constexpr std::string textureFileExt = ".bmp";

    // Add a texture by specifying a filename (don't include file extension or path).
    static void Add(const std::string& name, bool nullTexture = false) {
        std::string fullPath = (nullTexture) ? nullTexturePath + name + textureFileExt : texturesPath + name + textureFileExt;
        SDL_Surface* surface = SDL_LoadBMP((fullPath).c_str());
        if (!surface) {
            std::cout << "Surface loading at '" << texturesPath << name << textureFileExt << "' failed due to: '" << SDL_GetError() << "'.\n";
            return;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            std::cout << "Texture loading at '" << texturesPath << name << textureFileExt << "' failed due to: '" << SDL_GetError() << "'.\n";
            return;
        }

        // Make pixel-art look better without blurry lines.
        SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
        
        textures[name] = texture;

        SDL_DestroySurface(surface);
    }
};

// Short for TextureManager.
using TM = TextureManager;
