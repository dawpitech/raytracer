/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** MetalParser.cpp
*/

#include <iostream>

#include "Metal.hpp"
#include "MetalParser.hpp"
#include "RACIST/IModule.hpp"

std::unique_ptr<raytracer::engine::materials::IMaterial> raytracer::engine::materials::MetalParser::parseMaterial(const libconfig::Setting& materialConfig)
{
    try {
        const int r = materialConfig.lookup("r");
        const int g = materialConfig.lookup("g");
        const int b = materialConfig.lookup("b");

        const double fuzz = materialConfig.lookup("fuzz");

        return std::make_unique<Metal>(r, g, b, fuzz);
    } catch (libconfig::SettingNotFoundException& e){
        std::cerr << "Error occurred while parsing the configuration of material diffuse, "
                     "couldn't find " << e.getPath() << std::endl;
        throw MaterialParserException();
    }
}

std::vector<std::string> raytracer::engine::materials::MetalParser::getSupportedMaterials() { return {"metal"}; }

extern "C" raytracer::generic::ModuleVersion getModuleVersion() { return raytracer::generic::ModuleVersion::V1_0_0; }
extern "C" raytracer::generic::ModuleType getModuleType() { return raytracer::generic::ModuleType::MATERIAL; }
extern "C" std::unique_ptr<raytracer::generic::IMaterialParser> getMaterialParser() { return std::make_unique<raytracer::engine::materials::MetalParser>(); }
