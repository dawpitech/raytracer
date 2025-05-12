/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** DiffuseParser.cpp
*/

#include <iostream>

#include "DirectionalLightParser.hpp"
#include "DirectionalLight.hpp"
#include "RACIST/IModule.hpp"

std::unique_ptr<raytracer::engine::materials::IMaterial> raytracer::engine::materials::DirectionalLightParser::parseMaterial(const libconfig::Setting& materialConfig)
{
    try {
        const libconfig::Setting& color = materialConfig.lookup("color");
        const int r = color.lookup("r");
        const int g = color.lookup("g");
        const int b = color.lookup("b");
        const double intensity = materialConfig.lookup("intensity");

        return std::make_unique<DirectionalLight>(r, g, b, intensity);
    } catch (libconfig::SettingNotFoundException& e){
        std::cerr << "Error occurred while parsing the configuration of material diffuse, "
                     "couldn't find " << e.getPath() << std::endl;
        throw MaterialParserException();
    }
}

std::vector<std::string> raytracer::engine::materials::DirectionalLightParser::getSupportedMaterials() { return {"directionalLight"}; }

extern "C" raytracer::generic::ModuleVersion getModuleVersion() { return raytracer::generic::ModuleVersion::V1_0_0; }
extern "C" raytracer::generic::ModuleType getModuleType() { return raytracer::generic::ModuleType::MATERIAL; }
extern "C" std::unique_ptr<raytracer::generic::IMaterialParser> getMaterialParser() { return std::make_unique<raytracer::engine::materials::DirectionalLightParser>(); }
