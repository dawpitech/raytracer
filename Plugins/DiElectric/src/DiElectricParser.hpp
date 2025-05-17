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
    class DiElectricParser : public generic::IMaterialParser
    {
        public:
            DiElectricParser() = default;
            ~DiElectricParser() override = default;

            std::unique_ptr<materials::IMaterial> parseMaterial(const libconfig::Setting& materialConfig) override;
            std::vector<std::string> getSupportedMaterials() override;
    };
}
