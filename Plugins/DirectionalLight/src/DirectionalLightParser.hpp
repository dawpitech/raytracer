/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** DiffuseParser.hpp
*/

#pragma once

#include "RACIST/IMaterialParser.hpp"

namespace raytracer::engine::materials
{
    class DirectionalLightParser : public generic::IMaterialParser
    {
        public:
            DirectionalLightParser() = default;
            ~DirectionalLightParser() override = default;

            std::unique_ptr<IMaterial> parseMaterial(const libconfig::Setting& materialConfig) override;
            std::vector<std::string> getSupportedMaterials() override;
    };
}
