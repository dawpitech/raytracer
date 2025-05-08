/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** RendererFactory.hpp
*/

#pragma once

#include "PPMRenderer.hpp"

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
                throw UnknownRendererException();
            }
    };
}
