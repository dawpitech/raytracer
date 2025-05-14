/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ConeParser.cpp
*/

#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "ConeParser.hpp"
#include "Cone.hpp"
#include "RACIST/IModule.hpp"

std::unique_ptr<raytracer::engine::IObject> raytracer::engine::objects::cone::ConeParser::parseObject(
    const libconfig::Setting& objectConfig)
{
    try
    {
        const double px = objectConfig.lookup("position").lookup("x");
        const double py = objectConfig.lookup("position").lookup("y");
        const double pz = objectConfig.lookup("position").lookup("z");

        const double dx = objectConfig.lookup("direction").lookup("x");
        const double dy = objectConfig.lookup("direction").lookup("y");
        const double dz = objectConfig.lookup("direction").lookup("z");

        const double angle = objectConfig.lookup("angle");

        double heightValue;
        const libconfig::Setting& height = objectConfig.lookup("height");
        if (height.getType() == libconfig::Setting::TypeString && std::string(objectConfig.lookup("height")) == "inf") {
            heightValue = std::numeric_limits<double>::infinity();
        } else if (height.getType() == libconfig::Setting::TypeFloat) {
            heightValue = static_cast<double>(objectConfig.lookup("height"));
        } else {
            throw ObjectParserException();
        }

        return std::make_unique<Cone>(math::Point3D{px, py, pz}, math::Vec3<double>{dx, dy, dz}, angle, heightValue);
    } catch (libconfig::SettingNotFoundException& e) {
        std::cerr << "Error while parsing cone object, missing: " << e.getPath() << std::endl;
        throw ObjectParserException();
    }
}

std::vector<std::string> raytracer::engine::objects::cone::ConeParser::getSupportedObjects() { return {"cone"}; }
extern "C" raytracer::generic::ModuleVersion getModuleVersion() { return raytracer::generic::ModuleVersion::V1_0_0; }
extern "C" raytracer::generic::ModuleType getModuleType() { return raytracer::generic::ModuleType::PRIMITIVE; }
extern "C" std::unique_ptr<raytracer::generic::IObjectParser> getObjectParser() { return std::make_unique<raytracer::engine::objects::cone::ConeParser>(); }
