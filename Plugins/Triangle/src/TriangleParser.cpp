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
        
        double scale = 1.0;
        if (objectConfig.exists("size")) {
            scale = objectConfig.lookup("size");
        }
        
        math::Point3D v1, v2, v3;
        
        if (objectConfig.exists("vertices") && objectConfig.lookup("vertices").getLength() >= 3) {
            double v1rx = objectConfig.lookup("vertices")[0].lookup("x");
            double v1ry = objectConfig.lookup("vertices")[0].lookup("y");
            double v1rz = objectConfig.lookup("vertices")[0].lookup("z");
            
            double v2rx = objectConfig.lookup("vertices")[1].lookup("x");
            double v2ry = objectConfig.lookup("vertices")[1].lookup("y");
            double v2rz = objectConfig.lookup("vertices")[1].lookup("z");
            
            double v3rx = objectConfig.lookup("vertices")[2].lookup("x");
            double v3ry = objectConfig.lookup("vertices")[2].lookup("y");
            double v3rz = objectConfig.lookup("vertices")[2].lookup("z");
            
            v1 = math::Point3D{px + v1rx, py + v1ry, pz + v1rz};
            v2 = math::Point3D{px + v2rx, py + v2ry, pz + v2rz};
            v3 = math::Point3D{px + v3rx, py + v3ry, pz + v3rz};
        } else {
            v1 = math::Point3D{px - 0.5 * scale, py - 0.5 * scale, pz};
            v2 = math::Point3D{px + 0.5 * scale, py - 0.5 * scale, pz};
            v3 = math::Point3D{px, py + 0.5 * scale, pz};
        }

        return std::make_unique<Triangle>(v1, v2, v3);
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
