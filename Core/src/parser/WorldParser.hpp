/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** WorldParser.hpp
*/

#pragma once
#include "MasterParser.hpp"

namespace raytracer::parser
{
    class WorldParser
    {
        public:
            class WorldParserException final : public MasterParser::ParserException
            {
                public:
                    WorldParserException() = default;
                    explicit WorldParserException(const std::string& what)
                        :   ParserException(what) {}
            };

            WorldParser() = default;
            ~WorldParser() = default;

            static void parseWorldConfig(const libconfig::Setting& worldConfig, engine::WorldConfiguration& worldConfiguration);
    };
}
