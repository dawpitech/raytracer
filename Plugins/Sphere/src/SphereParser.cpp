/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SphereParser.cpp
*/

#include "SphereParser.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <RACIST/IModule.hpp>

#include "Sphere.hpp"

std::unique_ptr<raytracer::engine::IObject> raytracer::engine::objects::sphere::SphereParser::parseObject(const libconfig::Setting& objectConfig)
{
    try {
        const double radius = objectConfig.lookup("radius");
        const double x = objectConfig.lookup("position").lookup("x");
        const double y = objectConfig.lookup("position").lookup("y");
        const double z = objectConfig.lookup("position").lookup("z");

        return std::make_unique<Sphere>(math::Point3D{x, y, z}, radius);
    } catch (libconfig::SettingNotFoundException& e){
        std::cerr << "Error occurred while parsing the configuration of a sphere, "
                     "couldn't find " << e.getPath() << std::endl;
        throw ObjectParserException();
    }
}

std::vector<std::string> raytracer::engine::objects::sphere::SphereParser::getSupportedObjects() { return {"sphere", "ball"}; }

extern "C" raytracer::generic::ModuleVersion getModuleVersion() { return raytracer::generic::ModuleVersion::V1_0_0; }
extern "C" raytracer::generic::ModuleType getModuleType() { return raytracer::generic::ModuleType::PRIMITIVE; }
extern "C" std::unique_ptr<raytracer::generic::IObjectParser> getObjectParser() { return std::make_unique<raytracer::engine::objects::sphere::SphereParser>(); }
