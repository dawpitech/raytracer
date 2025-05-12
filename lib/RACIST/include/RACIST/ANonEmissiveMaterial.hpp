/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ANonEmissiveMaterial.hpp
*/

#pragma once

#include "IMaterial.hpp"

namespace raytracer::engine
{
    class ANonEmissiveMaterial : public materials::IMaterial
    {
        public:
            graphics::Color emittedColor() override { return graphics::Color{0, 0, 0}; }
    };
}
