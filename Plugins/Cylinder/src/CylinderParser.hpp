/*
** Raytracer
** File description:
** CylinderParser.hpp
*/

#pragma once

#include "RACIST/IObjectParser.hpp"

namespace raytracer::engine::objects::cylinder
{
    class CylinderParser : public generic::IObjectParser
    {
        public:
            CylinderParser() = default;
            ~CylinderParser() override = default;

            std::unique_ptr<IObject> parseObject(const libconfig::Setting& objectConfig) override;
            std::vector<std::string> getSupportedObjects() override;
    };
}
