/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** TriangleParser.hpp
*/

#pragma once

#include "RACIST/IObjectParser.hpp"

namespace raytracer::engine::objects::triangle
{
    class TriangleParser : public generic::IObjectParser
    {
        public:
            TriangleParser() = default;
            ~TriangleParser() override = default;

            std::unique_ptr<IObject> parseObject(const libconfig::Setting& objectConfig) override;
            std::vector<std::string> getSupportedObjects() override;
    };
}
