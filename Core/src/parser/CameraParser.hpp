/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** CameraParser.hpp
*/

#pragma once

#include "MasterParser.hpp"
#include "Raytracer.hpp"

namespace raytracer::parser
{
    class CameraParser
    {
        public:
            class CameraParserException final : public MasterParser::ParserException
            {
                public:
                    CameraParserException() = default;
                    explicit CameraParserException(const std::string& what)
                        :   ParserException(what) {}
            };

            CameraParser() = default;
            ~CameraParser() = default;

            static void parseCameraConfig(const libconfig::Setting& cameraConfig, engine::CameraMan& cameraMan);
    };
}
