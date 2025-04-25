/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** IRenderer.hpp
*/

#pragma once

namespace raytracer::graphics
{
    class IRenderer
    {
        public:
            class Exception final : public std::exception {};

            IRenderer() = default;
            virtual ~IRenderer() = default;

            virtual void renderCanva(const Canva& canva) const = 0;
            [[nodiscard]] virtual bool shouldExit() const = 0;
    };
}
