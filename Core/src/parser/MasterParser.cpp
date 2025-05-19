/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** MasterParser.cpp
*/

#include <iostream>
#include <libconfig.h++>

#include "MasterParser.hpp"

#include "CameraParser.hpp"
#include "WorldParser.hpp"

void raytracer::parser::MasterParser::parseScene(const std::string& filepath, Raytracer& raytracer, engine::CameraMan& cameraMan)
{
    libconfig::Config cfg;

    try {
        cfg.readFile(filepath.c_str());
    } catch(const libconfig::FileIOException&) {
        throw ParserException("I/O error while reading file.");
    } catch(const libconfig::ParseException &pex) {
        throw ParserException("Parse error at "
            + std::string(pex.getFile()) + ":"
            + std::to_string(pex.getLine()) + " - "
            + pex.getError());
    }

    try {
        std::cout << "[TRACE] Loading scene configuration file '" << filepath << "'" << std::endl;
        const libconfig::Setting& root = cfg.getRoot();
        const libconfig::Setting& objects = root.lookup("objects");
        const libconfig::Setting& camera = root.lookup("camera");
        const libconfig::Setting& world = root.lookup("world");

        std::clog << "[TRACE] Loading camera configuration" << std::endl;
        CameraParser::parseCameraConfig(camera, cameraMan);
        std::clog << "[TRACE] Camera configuration loaded" << std::endl;

        std::clog << "[TRACE] Loading world configuration" << std::endl;
        WorldParser::parseWorldConfig(world, raytracer.getWorldConfig());
        std::clog << "[TRACE] World configuration loaded" << std::endl;

        try {
            const int objectsNumber = objects.getLength();
            std::cout << "[TRACE] Objects found: " << objectsNumber << std::endl;

            for (int i = 0; i < objectsNumber; i++) {
                const libconfig::Setting& obj = objects[i];
                const std::string objType = obj.lookup("type");

                const libconfig::Setting& objMaterial = obj.lookup("material");
                const std::string objMaterialType = objMaterial.lookup("type");

                std::clog << "[TRACE] Loading object with type '" << objType << "' and material '" << objMaterialType << "'" << std::endl;

                try {
                    auto engineObject = raytracer.getObjectsParser().at(objType)->parseObject(obj);

                    try {
                        auto engineMaterial = raytracer.getMaterialsParser().at(objMaterialType)->parseMaterial(objMaterial);

                        engineObject->setMaterial(engineMaterial);
                    } catch (const std::out_of_range&) {
                        throw ParserException("No valid plugin for material '" + objMaterialType + "'");
                    }

                    raytracer.getMainScene().add(engineObject);
                } catch (const std::out_of_range&) {
                    throw ParserException("No valid plugin for primitive '" + objType + "'");
                }

                std::clog << "[TRACE] Object loaded" << std::endl;
            }
        } catch(libconfig::SettingNotFoundException& snfe) {
            throw ParserException("Parser reached an invalid object, missing: " + std::string(snfe.getPath()));
        }
    } catch (libconfig::SettingNotFoundException& snfe) {
        throw ParserException("Required sections missing: " + std::string(snfe.getPath()));
    }
    std::clog << "[TRACE] Scene fully loaded" << std::endl;
}
