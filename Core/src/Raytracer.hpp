/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Raytracer.hpp
*/

#pragma once

#include <map>
#include <vector>

#include "engine/Camera.hpp"
#include "plugins/SafeDL.hpp"
#include "RACIST/IObjectParser.hpp"

namespace raytracer
{
    class Raytracer
    {
        public:
            Raytracer() : _camera(1, 400) {}
            ~Raytracer() = default;

            void loadPlugins();
            int parseSceneConfig(const std::string& filepath);
            void run();

            [[nodiscard]] const std::map<std::string, std::unique_ptr<generic::IObjectParser>>& getObjectsParser() const;
            engine::Scene& getMainScene();
            engine::Camera& getMainCamera();

        private:
            std::vector<SafeDL::safeHandle> _pluginInventory;
            std::map<std::string, std::unique_ptr<generic::IObjectParser>> _objectsParser;
            engine::Camera _camera;
            engine::Scene _world;
            std::unique_ptr<graphics::IRenderer> _renderer;
    };
}
