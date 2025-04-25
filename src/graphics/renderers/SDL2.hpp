/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SDL2.hpp
*/

#pragma once

#include <memory>

#ifdef EMSCRIPTEN_BUILD
    #include <SDL.h>
#else
    #include <SDL2/SDL.h>
#endif

#include "graphics/Canva.hpp"

namespace raytracer::graphics
{
    class SDLRenderer
    {
        public:
            class Exception final : public std::exception {};

            struct WindowDeleter {
                void operator()(SDL_Window* window) const {
                    if (window) SDL_DestroyWindow(window);
                }
            };

            struct RendererDeleter {
                void operator()(SDL_Renderer* renderer) const {
                    if (renderer) SDL_DestroyRenderer(renderer);
                }
            };

            struct TextureDeleter {
                void operator()(SDL_Texture* texture) const {
                    if (texture) SDL_DestroyTexture(texture);
                }
            };

            SDLRenderer()
            {
                if (SDL_Init(SDL_INIT_VIDEO) < 0)
                    throw Exception();

                this->_window.reset(SDL_CreateWindow(
                    "Raytracer",
                    SDL_WINDOWPOS_UNDEFINED,
                    SDL_WINDOWPOS_UNDEFINED,
                    IMAGE_WIDTH,
                    IMAGE_HEIGHT,
                    SDL_WINDOW_SHOWN)
                );

                if (this->_window == nullptr)
                    throw Exception();

                this->_renderer.reset(SDL_CreateRenderer(
                    this->_window.get(),
                    -1,
                    SDL_RENDERER_ACCELERATED)
                );

                if (this->_renderer == nullptr)
                    throw Exception();

                this->_texture.reset(SDL_CreateTexture(
                    this->_renderer.get(),
                    SDL_PIXELFORMAT_RGBA8888,
                    SDL_TEXTUREACCESS_TARGET,
                    IMAGE_WIDTH,
                    IMAGE_HEIGHT)
                );

                if (this->_texture == nullptr)
                    throw Exception();

                SDL_SetRenderTarget(this->_renderer.get(), this->_texture.get());

                SDL_SetRenderDrawColor(this->_renderer.get(), 0, 0, 0, 255);
                SDL_RenderClear(this->_renderer.get());

                SDL_SetRenderTarget(this->_renderer.get(), nullptr);

                SDL_RenderCopy(this->_renderer.get(), this->_texture.get(), nullptr, nullptr);
                SDL_RenderPresent(this->_renderer.get());
            }

            ~SDLRenderer() { SDL_Quit(); }

            void renderCanva(const Canva& canva) const
            {
                // Variables for texture locking
                void* pixelData;
                int pitch;

                // Lock texture for pixel access
                SDL_LockTexture(_texture.get(), nullptr, &pixelData, &pitch);

                // Cast pointer to make it easier to work with pixels
                auto* pixels = static_cast<uint32_t*>(pixelData);

                // Copy pixels from canva to texture
                for (int j = 0; j < IMAGE_HEIGHT; j++) {
                    for (int i = 0; i < IMAGE_WIDTH; i++) {
                        const Color& color = canva.getPixelColor(i, j);
                        uint32_t pixelColor = SDL_MapRGBA(
                            SDL_GetWindowSurface(_window.get())->format,
                            color.rScaled(), color.gScaled(), color.bScaled(), 255
                        );
                        pixels[j * (pitch / sizeof(uint32_t)) + i] = pixelColor;
                    }
                }

                // Unlock texture
                SDL_UnlockTexture(_texture.get());

                // Render the texture once
                SDL_RenderClear(_renderer.get());
                SDL_RenderCopy(_renderer.get(), _texture.get(), nullptr, nullptr);
                SDL_RenderPresent(_renderer.get());
            }

            static bool shouldExit()
            {
                SDL_Event event;
                while (SDL_PollEvent(&event))
                {
                    if (event.type == SDL_QUIT)
                        return true;
                }
                return false;
            }

        private:
            std::unique_ptr<SDL_Window, WindowDeleter> _window;
            std::unique_ptr<SDL_Renderer, RendererDeleter> _renderer;
            std::unique_ptr<SDL_Texture, TextureDeleter> _texture;
    };
}
