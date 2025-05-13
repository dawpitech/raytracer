/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** RaytracerConfiguration.hpp
*/

#pragma once

namespace raytracer
{
    class RaytracerConfiguration
    {
        public:
            std::string sceneConfigurationFilePath;
            bool watchingConfig = false;
            std::string renderer = "PPM";
            bool multithreading = true;
            unsigned int threadCount = 0;
    };
}
