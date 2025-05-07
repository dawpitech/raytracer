/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** CameraParser.cpp
*/

#include "CameraParser.hpp"

void raytracer::parser::CameraParser::parseCameraConfig(const libconfig::Setting& cameraConfig, engine::Camera& camera)
{
    try {
        const libconfig::Setting& resolution = cameraConfig.lookup("resolution");
        const int width = resolution.lookup("width");
        const int height = resolution.lookup("height");

        const libconfig::Setting& position = cameraConfig.lookup("position");
        const double pos_x = position.lookup("x");
        const double pos_y = position.lookup("y");
        const double pos_e = position.lookup("z");

        const libconfig::Setting& rotation = cameraConfig.lookup("rotation");
        const double rot_x = position.lookup("x");
        const double rot_y = position.lookup("y");
        const double rot_e = position.lookup("z");

        const libconfig::Setting& fov = cameraConfig.lookup("fieldOfView");

        camera.setImageWidth(width);
        camera.setAspectRatio(static_cast<double>(width) / height);
    } catch (libconfig::SettingNotFoundException& snfe) {
        throw CameraParserException("Required field of camera not found: " + std::string(snfe.getPath()));
    }
}
