/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** IMaterialParser.hpp
*/

#pragma once

#include <exception>
#include <libconfig.h++>
#include <memory>
#include <vector>

#include "IMaterial.hpp"

namespace raytracer::generic
{
    class IMaterialParser
    {
        public:
            class MaterialParserException : public std::exception {};

            IMaterialParser() = default;
            virtual ~IMaterialParser() = default;

            virtual std::unique_ptr<engine::materials::IMaterial> parseMaterial(const libconfig::Setting& materialConfig) = 0;
            virtual std::vector<std::string> getSupportedMaterials() = 0;
    };
}

extern "C" std::unique_ptr<raytracer::generic::IMaterialParser> getMaterialParser(void);
