/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PlaneParser.cpp
*/

#include "PlaneParser.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <RACIST/IModule.hpp>

#include "Plane.hpp"

#include <cmath>

std::unique_ptr<raytracer::engine::IObject> raytracer::engine::objects::plane::PlaneParser::parseObject(
    const libconfig::Setting& objectConfig)
{
    try
    {
        const double px = objectConfig.lookup("position").lookup("x");
        const double py = objectConfig.lookup("position").lookup("y");
        const double pz = objectConfig.lookup("position").lookup("z");

        const double angleX_deg = objectConfig.lookup("rotation").lookup("x");
        const double angleY_deg = objectConfig.lookup("rotation").lookup("y");

        const double angleX = angleX_deg * M_PI / 180.0;
        const double angleY = angleY_deg * M_PI / 180.0;

        const double nx = cos(angleX) * sin(angleY);
        const double ny = sin(angleX);
        const double nz = cos(angleX) * cos(angleY);

        return std::make_unique<Plane>(math::Point3D{px, py, pz}, math::Vec3{nx, ny, nz});
    }
    catch (libconfig::SettingNotFoundException& e)
    {
        std::cerr << "Error occurred while parsing the configuration of a plane, "
                  << "couldn't find " << e.getPath() << std::endl;
        throw ObjectParserException();
    }
}

std::vector<std::string> raytracer::engine::objects::plane::PlaneParser::getSupportedObjects()
{
    return {"plane", "flat"};
}

extern "C" raytracer::generic::ModuleVersion getModuleVersion() { return raytracer::generic::ModuleVersion::V1_0_0; }
extern "C" raytracer::generic::ModuleType getModuleType() { return raytracer::generic::ModuleType::PRIMITIVE; }

extern "C" std::unique_ptr<raytracer::generic::IObjectParser> getObjectParser()
{
    return std::make_unique<raytracer::engine::objects::plane::PlaneParser>();
}
