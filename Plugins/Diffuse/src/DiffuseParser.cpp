/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** DiffuseParser.cpp
*/

#include <iostream>

#include "DiffuseParser.hpp"
#include "Diffuse.hpp"
#include "RACIST/IModule.hpp"

std::unique_ptr<raytracer::engine::materials::IMaterial> raytracer::engine::materials::DiffuseParser::parseMaterial(const libconfig::Setting& materialConfig)
{
    try {
        const int r = materialConfig.lookup("r");
        const int g = materialConfig.lookup("g");
        const int b = materialConfig.lookup("b");

        const double rScaled = static_cast<double>(r) / 255;
        const double gScaled = static_cast<double>(g) / 255;
        const double bScaled = static_cast<double>(b) / 255;

        return std::make_unique<Diffuse>(graphics::Color{rScaled, gScaled, bScaled});
    } catch (libconfig::SettingNotFoundException& e){
        std::cerr << "Error occurred while parsing the configuration of material diffuse, "
                     "couldn't find " << e.getPath() << std::endl;
        throw MaterialParserException();
    }
}

std::vector<std::string> raytracer::engine::materials::DiffuseParser::getSupportedMaterials() { return {"diffuse"}; }

extern "C" raytracer::generic::ModuleVersion getModuleVersion() { return raytracer::generic::ModuleVersion::V1_0_0; }
extern "C" raytracer::generic::ModuleType getModuleType() { return raytracer::generic::ModuleType::MATERIAL; }
extern "C" std::unique_ptr<raytracer::generic::IMaterialParser> getMaterialParser() { return std::make_unique<raytracer::engine::materials::DiffuseParser>(); }
