/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** WorldConfiguration.hpp
*/

#pragma once

namespace raytracer::engine
{
    class WorldConfiguration
    {
        public:
            WorldConfiguration() = default;
            ~WorldConfiguration() = default;

            bool skyboxEnabled = false;
            graphics::Color ambientLight{1, 1, 1};
            double ambientLightIntensity = 1.0;
    };
}
