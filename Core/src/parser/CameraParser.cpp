/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** CameraParser.cpp
*/

#include "CameraParser.hpp"

void raytracer::parser::CameraParser::parseCameraConfig(const libconfig::Setting& cameraConfig, engine::CameraMan& cameraMan)
{
    try {
        const bool vrRender = cameraConfig.lookup("vrRender");
        if (vrRender)
            cameraMan.switchToVR();

        const libconfig::Setting& resolution = cameraConfig.lookup("resolution");
        const int width = resolution.lookup("width");
        const int height = resolution.lookup("height");

        const libconfig::Setting& position = cameraConfig.lookup("position");
        const double posX = position.lookup("x");
        const double posY = position.lookup("y");
        const double posZ = position.lookup("z");

        const libconfig::Setting& rotation = cameraConfig.lookup("rotation");
        const double rotX = rotation.lookup("x");
        const double rotY = rotation.lookup("y");
        const double rotZ = rotation.lookup("z");

        const double fov = cameraConfig.lookup("fieldOfView");
        const int sampleRate = cameraConfig.lookup("sampleRate");

        for (const auto& camera : cameraMan.getCameras()) {
            camera->setImageWidth(width);
            camera->setSampleRate(sampleRate);
            camera->setFOV(fov);
            camera->setPosition(math::Point3D{posX, posY, posZ});
            camera->setRotation(math::Vec3{rotX, rotY, rotZ});
        }

        if (vrRender) {
            const libconfig::Setting& vrRenderSettings = cameraConfig.lookup("vrRenderSettings");

            const libconfig::Setting& positionOffset = vrRenderSettings.lookup("positionOffset");
            const double offset_x = positionOffset.lookup("x");
            const double offset_y = positionOffset.lookup("y");
            const double offset_z = positionOffset.lookup("z");
            const auto offset = math::Vec3{offset_x, offset_y, offset_z};

            cameraMan.getCameras().at(0)->setPosition(math::Point3D(math::Point3D{posX, posY, posZ} - offset / 2));
            cameraMan.getCameras().at(1)->setPosition(math::Point3D(math::Point3D{posX, posY, posZ} + offset / 2));
        } else {
            cameraMan.getCameras().at(0)->setAspectRatio(static_cast<double>(width) / height);
        }
        cameraMan.updateCanvaSize(width);
    } catch (libconfig::SettingNotFoundException& snfe) {
        throw CameraParserException("Required field of camera not found: " + std::string(snfe.getPath()));
    }
}
