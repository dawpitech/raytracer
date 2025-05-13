/*
** Raytracer
** File description:
** PlaneParser.hpp
*/

#pragma once

#include "RACIST/IObjectParser.hpp"

namespace raytracer::engine::objects::plane
{
  class PlaneParser : public generic::IObjectParser
  {
    public:
      PlaneParser() = default;
      ~PlaneParser() override = default;

      std::unique_ptr<IObject> parseObject(const libconfig::Setting& objectConfig) override;
      std::vector<std::string> getSupportedObjects() override;
  };
}