/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SFMLRenderer.hpp
*/

#pragma once

#include <SFML/Graphics.hpp>

#include "IRenderer.hpp"

namespace raytracer::graphics
{
    class SFMLRenderer final : public IRenderer
    {
        public:
            SFMLRenderer();
            ~SFMLRenderer() override;

            void renderCanva(const Canva& canva) override;
            void renderLine(const Canva& canva, int y) override;

            [[nodiscard]] std::string getName() const override { return "SFML Live Renderer"; }

            [[nodiscard]] bool exitRequested() override;
            [[nodiscard]] bool isInteractive() const override { return true; }

        private:
            sf::RenderWindow _window;
    };
}
