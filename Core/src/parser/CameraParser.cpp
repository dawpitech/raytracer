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
        const bool vrRender = cameraConfig.lookup("vrRender");

        const libconfig::Setting& resolution = cameraConfig.lookup("resolution");
        const int width = resolution.lookup("width");
        const int height = resolution.lookup("height");

        const libconfig::Setting& position = cameraConfig.lookup("position");
        const double pos_x = position.lookup("x");
        const double pos_y = position.lookup("y");
        const double pos_z = position.lookup("z");

        const libconfig::Setting& rotation = cameraConfig.lookup("rotation");
        const double rot_x = position.lookup("x");
        const double rot_y = position.lookup("y");
        const double rot_z = position.lookup("z");

        const double fov = cameraConfig.lookup("fieldOfView");
        const int sampleRate = cameraConfig.lookup("sampleRate");

        camera.setImageWidth(width);
        camera.setSampleRate(sampleRate);
        camera.setFOV(fov);
        camera.setPosition(math::Point3D{pos_x, pos_y, pos_z});

        if (vrRender) {
            const libconfig::Setting& vrRenderSettings = cameraConfig.lookup("vrRenderSettings");

            const libconfig::Setting& positionOffset = vrRenderSettings.lookup("positionOffset");
            const double offset_x = position.lookup("x");
            const double offset_y = position.lookup("y");
            const double offset_e = position.lookup("z");

            camera.setAspectRatio(1.0);
        } else {
            camera.setAspectRatio(static_cast<double>(width) / height);
        }
    } catch (libconfig::SettingNotFoundException& snfe) {
        throw CameraParserException("Required field of camera not found: " + std::string(snfe.getPath()));
    }
}
