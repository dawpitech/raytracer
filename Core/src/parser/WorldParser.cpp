/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** WorldParser.cpp
*/

#include "WorldParser.hpp"

void raytracer::parser::WorldParser::parseWorldConfig(const libconfig::Setting& worldConfig, engine::WorldConfiguration& worldConfiguration)
{
    try {
        const bool skyboxEnabled = worldConfig.lookup("skybox");
        const double ambientLightIntensity = worldConfig.lookup("ambientLightIntensity");

        const libconfig::Setting& ambientLightColor = worldConfig.lookup("ambientLightColor");
        const int r = ambientLightColor.lookup("r");
        const int g = ambientLightColor.lookup("g");
        const int b = ambientLightColor.lookup("b");

        worldConfiguration.skyboxEnabled = skyboxEnabled;
        worldConfiguration.ambientLightIntensity = ambientLightIntensity;
        worldConfiguration.ambientLight = graphics::Color{
            static_cast<double>(r) / 255,
            static_cast<double>(g) / 255,
            static_cast<double>(b) / 255
        };
    } catch (libconfig::SettingNotFoundException& snfe) {
        throw WorldParserException("Required field of camera not found: " + std::string(snfe.getPath()));
    }
}
