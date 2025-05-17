/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** RendererFactory.hpp
*/

#pragma once

#include "PPMRenderer.hpp"
#include "SDLRenderer.hpp"
#include "SFMLRenderer.hpp"

namespace raytracer::graphics
{
    class RendererFactory
    {
        public:
            class UnknownRendererException final : public std::exception
            {
                public:
                    [[nodiscard]] const char* what() const noexcept override
                    {
                        return "Unknown renderer";
                    }
            };

            static std::unique_ptr<IRenderer> createRenderer(const std::string& name)
            {
                if (name == "PPM")
                    return std::make_unique<PPMRenderer>();
                if (name == "SDL")
                    return std::make_unique<SDLRenderer>();
                if (name == "SFML")
                    return std::make_unique<SFMLRenderer>();
                throw UnknownRendererException();
            }
    };
}
