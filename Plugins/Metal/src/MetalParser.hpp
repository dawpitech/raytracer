/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** MetalParser.hpp
*/

#pragma once

#include "RACIST/IMaterialParser.hpp"

namespace raytracer::engine::materials
{
    class MetalParser : public generic::IMaterialParser
    {
        public:
            MetalParser() = default;
            ~MetalParser() override = default;

            std::unique_ptr<IMaterial> parseMaterial(const libconfig::Setting& materialConfig) override;
            std::vector<std::string> getSupportedMaterials() override;
    };
}
