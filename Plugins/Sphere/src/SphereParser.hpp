/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SphereParser.hpp
*/

#pragma once

#include "RACIST/IObjectParser.hpp"

namespace raytracer::engine::objects::sphere
{
    class SphereParser : public generic::IObjectParser
    {
        public:
            SphereParser() = default;
            ~SphereParser() override = default;

            std::unique_ptr<IObject> parseObject(const libconfig::Setting& objectConfig) override;
            std::vector<std::string> getSupportedObjects() override;
    };
}
