/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** WorldParser.cpp
*/

#include "WorldParser.hpp"

void raytracer::parser::WorldParser::parseWorldConfig(const libconfig::Setting& worldConfig, engine::WorldConfiguration& engineWorldConfig)
{
    try {
        const libconfig::Setting& skybox = worldConfig.lookup("skybox");
        engineWorldConfig.skybox.enabled = skybox.lookup("enabled");
        engineWorldConfig.skybox.illuminate = skybox.lookup("illuminate");

        const libconfig::Setting& ambientLight = worldConfig.lookup("ambientLight");
        const libconfig::Setting& ambientLightColor = ambientLight.lookup("color");
        const int r = ambientLightColor.lookup("r");
        const int g = ambientLightColor.lookup("g");
        const int b = ambientLightColor.lookup("b");
        engineWorldConfig.ambientLight.enabled = ambientLight.lookup("enabled");
        engineWorldConfig.ambientLight.intensity = ambientLight.lookup("intensity");

        engineWorldConfig.ambientLight.color = graphics::Color{
            static_cast<double>(r) / 255,
            static_cast<double>(g) / 255,
            static_cast<double>(b) / 255
        };
    } catch (libconfig::SettingNotFoundException& snfe) {
        throw WorldParserException("Required field of camera not found: " + std::string(snfe.getPath()));
    }
}
