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

std::unique_ptr<raytracer::engine::materials::IMaterial> raytracer::engine::materials::DiffuseParser::parseObject(const libconfig::Setting& materialConfig)
{
    try {
        const double r = materialConfig.lookup("position").lookup("r");
        const double g = materialConfig.lookup("position").lookup("g");
        const double b = materialConfig.lookup("position").lookup("b");

        return std::make_unique<materials::Diffuse>(graphics::Color{r, g, b});
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
