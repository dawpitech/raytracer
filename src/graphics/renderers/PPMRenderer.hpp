/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PPMRenderer.hpp
*/

#pragma once

#include "graphics/Canva.hpp"
#include "graphics/renderers/IRenderer.hpp"

namespace raytracer::graphics
{
    class PPMRenderer final : public IRenderer
    {
        public:
            PPMRenderer() = default;
            ~PPMRenderer() override = default;

            void renderCanva(const Canva& canva) const override;

            [[nodiscard]] bool shouldExit() const override { return true; }
    };
}
