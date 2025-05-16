/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Raytracer.hpp
*/

#pragma once

#include <map>
#include <vector>
#include <bits/fs_fwd.h>

#include "RaytracerConfiguration.hpp"
#include "engine/Camera.hpp"
#include "engine/CameraMan.hpp"
#include "engine/WorldConfiguration.hpp"
#include "plugins/SafeDL.hpp"
#include "RACIST/IMaterialParser.hpp"
#include "RACIST/IObjectParser.hpp"

namespace raytracer
{
    class Raytracer
    {
        public:
            Raytracer()
            {
                _cameraMan = std::make_unique<engine::CameraMan>();
            }
            ~Raytracer() = default;

            int parseArgs(int argc, const char **argv);
            void loadPlugins();
            int parseSceneConfig();
            void run();

            [[nodiscard]] const std::map<std::string, std::unique_ptr<generic::IObjectParser>>& getObjectsParser() const;
            [[nodiscard]] const std::map<std::string, std::unique_ptr<generic::IMaterialParser>>& getMaterialsParser() const;
            engine::Scene& getMainScene();
            engine::WorldConfiguration& getWorldConfig();

        private:
            static void printHelp();
            int render() const;

            std::vector<SafeDL::safeHandle> _pluginInventory;
            std::map<std::string, std::unique_ptr<generic::IObjectParser>> _objectsParser;
            std::map<std::string, std::unique_ptr<generic::IMaterialParser>> _materialsParser;
            std::unique_ptr<engine::CameraMan> _cameraMan;
            engine::Scene _world;
            std::unique_ptr<graphics::IRenderer> _renderer;
            engine::WorldConfiguration _worldConfig;
            RaytracerConfiguration _config;
            std::filesystem::file_time_type _configFileLastEditTime;
    };
}
