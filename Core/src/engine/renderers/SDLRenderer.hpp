/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SDLRenderer.hpp
*/

#pragma once

#include <SDL2/SDL.h>

#include "IRenderer.hpp"

namespace raytracer::graphics
{
    class SDLRenderer final : public IRenderer
    {
        public:
            SDLRenderer();
            ~SDLRenderer() override = default;

            void renderCanva(const Canva& canva) const override { throw Exception(); }
            void renderLine(const Canva& canva, int y) override;

            [[nodiscard]] std::string getName() const override { return "SDL2 Live Renderer"; }

            [[nodiscard]] bool exitRequested() const override;
            [[nodiscard]] bool isInteractive() const override { return true; }

        private:
            SDL_Window* _window;
            SDL_Renderer* _renderer;
            SDL_Texture* _texture;
    };
}
