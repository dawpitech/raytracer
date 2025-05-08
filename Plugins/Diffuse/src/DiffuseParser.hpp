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
    class DiffuseParser : public generic::IMaterialParser
    {
        public:
            DiffuseParser() = default;
            ~DiffuseParser() override = default;

            std::unique_ptr<materials::IMaterial> parseObject(const libconfig::Setting& materialConfig) override;
            std::vector<std::string> getSupportedMaterials() override;
    };
}
