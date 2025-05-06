/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** IObjectParser.hpp
*/

#pragma once

#include <exception>
#include <libconfig.h++>
#include <memory>
#include <vector>

#include "IObject.hpp"

namespace raytracer::generic
{
    class IObjectParser
    {
        public:
            class ObjectParserException : public std::exception {};

            IObjectParser() = default;
            virtual ~IObjectParser() = default;

            virtual std::unique_ptr<engine::IObject> parseObject(const libconfig::Setting& objectConfig) = 0;
            virtual std::vector<std::string> getSupportedObjects() = 0;
    };
}

extern "C" std::unique_ptr<raytracer::generic::IObjectParser> getObjectParser(void);
