/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PPMRenderer.hpp
*/

#pragma once

#include "IRenderer.hpp"
#include "../Canva.hpp"

namespace raytracer::graphics
{
    class PPMRenderer final : public IRenderer
    {
        public:
            PPMRenderer() = default;
            ~PPMRenderer() override = default;

            void renderCanva(const Canva& canva) override;
            void renderLine(const Canva& canva, int y) override { throw Exception(); }

            [[nodiscard]] std::string getName() const override { return "PPM File Renderer"; }

            [[nodiscard]] bool exitRequested() override { return false; }
            [[nodiscard]] bool isInteractive() const override { return false; }
    };
}
