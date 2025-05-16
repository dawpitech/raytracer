/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** DiffuseParser.cpp
*/

#include <iostream>

#include "DiElectricParser.hpp"
#include "DiElectric.hpp"
#include "RACIST/IModule.hpp"

std::unique_ptr<raytracer::engine::materials::IMaterial> raytracer::engine::materials::DiElectricParser::parseMaterial(const libconfig::Setting& materialConfig)
{
    try {
        double RI;
        bool perfect = false;
        const std::string type = materialConfig.lookup("type");

        if (type == "glass") {
            RI = 1.5;
        } else if (type == "air") {
            RI = 1.0;
        } else if (type == "water") {
            RI = 1.33;
        } else {
            RI = materialConfig.lookup("reflectingIndex");
            perfect = materialConfig.lookup("isPerfect");
        }

        return std::make_unique<DiElectric>(RI, perfect);
    } catch (libconfig::SettingNotFoundException& e){
        std::cerr << "Error occurred while parsing the configuration of material dielectric, "
                     "couldn't find " << e.getPath() << std::endl;
        throw MaterialParserException();
    }
}

std::vector<std::string> raytracer::engine::materials::DiElectricParser::getSupportedMaterials() { return {"dielectric", "glass", "air", "water"}; }

extern "C" raytracer::generic::ModuleVersion getModuleVersion() { return raytracer::generic::ModuleVersion::V1_0_0; }
extern "C" raytracer::generic::ModuleType getModuleType() { return raytracer::generic::ModuleType::MATERIAL; }
extern "C" std::unique_ptr<raytracer::generic::IMaterialParser> getMaterialParser() { return std::make_unique<raytracer::engine::materials::DiElectricParser>(); }
