/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** MasterParser.hpp
*/

#pragma once

#include <string>

#include "Raytracer.hpp"
#include "engine/CameraMan.hpp"

namespace raytracer::parser
{
    class MasterParser
    {
        public:
            // ReSharper disable once CppClassCanBeFinal
            class ParserException : public std::exception
            {
                public:
                    explicit ParserException(const std::string& what = "Unknown parser exception") { this->_what = what; }
                    [[nodiscard]] const char* what() const noexcept override { return this->_what.c_str(); }

                private:
                    std::string _what;
            };

            MasterParser() = default;
            ~MasterParser() = default;

            static void parseScene(const std::string& filepath, Raytracer& raytracer, engine::CameraMan& cameraMan);
    };
}
