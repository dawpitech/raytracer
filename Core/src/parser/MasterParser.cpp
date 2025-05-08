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

void raytracer::parser::MasterParser::parseScene(const std::string& filepath, Raytracer& raytracer)
{
    libconfig::Config cfg;

    try {
        cfg.readFile(filepath);
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

        std::cout << "[TRACE] Loading camera configuration" << std::endl;
        CameraParser::parseCameraConfig(camera, raytracer.getMainCamera());

        try {
            const int objectsNumber = objects.getLength();
            std::cout << "[TRACE] Objects found: " << objectsNumber << std::endl;

            for (int i = 0; i < objectsNumber; i++) {
                const libconfig::Setting& obj = objects[i];
                const std::string obj_type = obj.lookup("type");

                std::clog << "[TRACE] Loading object with type '" << obj_type << "'" << std::endl;

                try {
                    auto engineObject = raytracer.getObjectsParser().at(obj_type)->parseObject(obj);
                    raytracer.getMainScene().add(engineObject);
                } catch (const std::out_of_range&) {
                    throw ParserException("No valid plugin for parsing type " + obj_type);
                }
            }
        } catch(libconfig::SettingNotFoundException& snfe) {
            throw ParserException("Parser reached an invalid object, missing: " + std::string(snfe.getPath()));
        }
    } catch (libconfig::SettingNotFoundException& snfe) {
        throw ParserException("Required sections missing: " + std::string(snfe.getPath()));
    }
}
