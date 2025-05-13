/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** TriangleParser.cpp
*/

#include "TriangleParser.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <RACIST/IModule.hpp>

#include "Triangle.hpp"

std::unique_ptr<raytracer::engine::IObject> raytracer::engine::objects::triangle::TriangleParser::parseObject(const libconfig::Setting& objectConfig)
{
    try {
        const double px = objectConfig.lookup("position").lookup("x");
        const double py = objectConfig.lookup("position").lookup("y");
        const double pz = objectConfig.lookup("position").lookup("z");
        
        double v1x = px - 0.5, v1y = py - 0.5, v1z = pz;
        double v2x = px + 0.5, v2y = py - 0.5, v2z = pz;
        double v3x = px, v3y = py + 0.5, v3z = pz;
        
        if (objectConfig.exists("vertices")) {
            if (objectConfig.lookup("vertices").getLength() >= 3) {
                v1x = objectConfig.lookup("vertices")[0].lookup("x");
                v1y = objectConfig.lookup("vertices")[0].lookup("y");
                v1z = objectConfig.lookup("vertices")[0].lookup("z");
                
                v2x = objectConfig.lookup("vertices")[1].lookup("x");
                v2y = objectConfig.lookup("vertices")[1].lookup("y");
                v2z = objectConfig.lookup("vertices")[1].lookup("z");
                
                v3x = objectConfig.lookup("vertices")[2].lookup("x");
                v3y = objectConfig.lookup("vertices")[2].lookup("y");
                v3z = objectConfig.lookup("vertices")[2].lookup("z");
            }
        }

        return std::make_unique<Triangle>(
            math::Point3D{v1x, v1y, v1z},
            math::Point3D{v2x, v2y, v2z},
            math::Point3D{v3x, v3y, v3z}
        );
    } catch (libconfig::SettingNotFoundException& e) {
        std::cerr << "Error occurred while parsing the configuration of a triangle, "
                     "couldn't find " << e.getPath() << std::endl;
        throw ObjectParserException();
    }
}

std::vector<std::string> raytracer::engine::objects::triangle::TriangleParser::getSupportedObjects() { return {"triangle"}; }

extern "C" raytracer::generic::ModuleVersion getModuleVersion() { return raytracer::generic::ModuleVersion::V1_0_0; }
extern "C" raytracer::generic::ModuleType getModuleType() { return raytracer::generic::ModuleType::PRIMITIVE; }
extern "C" std::unique_ptr<raytracer::generic::IObjectParser> getObjectParser() { return std::make_unique<raytracer::engine::objects::triangle::TriangleParser>(); }
