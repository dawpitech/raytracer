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
            class Skybox
            {
                public:
                    bool enabled = true;
                    bool illuminate = true;
            };
            class AmbientLight
            {
                public:
                    bool enabled = true;
                    double intensity = 0.2;
                    graphics::Color color{1, 1, 1};
            };

            WorldConfiguration() = default;
            ~WorldConfiguration() = default;

            Skybox skybox;
            AmbientLight ambientLight;
    };
}
