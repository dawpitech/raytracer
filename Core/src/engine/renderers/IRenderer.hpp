/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** IRenderer.hpp
*/

#pragma once

#include "engine/Canva.hpp"

namespace raytracer::graphics
{
    class IRenderer
    {
        public:
            class Exception final : public std::exception {};

            IRenderer() = default;
            virtual ~IRenderer() = default;

            // Should only be implemented on NON-INTERACTIVE renderers
            virtual void renderCanva(const Canva& canva) const = 0;
            // Should only be implemented on INTERACTIVE renderers
            virtual void renderLine(const Canva& canva, int y) = 0;

            [[nodiscard]] virtual std::string getName() const = 0;

            [[nodiscard]] virtual bool exitRequested() const = 0;
            [[nodiscard]] virtual bool isInteractive() const = 0;

    };
}
