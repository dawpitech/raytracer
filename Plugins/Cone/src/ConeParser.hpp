/*
** Raytracer
** File description:
** ConeParser.hpp
*/

#pragma once

#include "RACIST/IObjectParser.hpp"

namespace raytracer::engine::objects::cone
{
  class ConeParser : public generic::IObjectParser
  {
    public:
      ConeParser() = default;
      ~ConeParser() override = default;

      std::unique_ptr<IObject> parseObject(const libconfig::Setting& objectConfig) override;
      std::vector<std::string> getSupportedObjects() override;
  };
}
