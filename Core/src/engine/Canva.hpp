/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Canva.hpp
*/

#pragma once

#include "MATH/Matrix.hpp"
#include "RACIST/Color.hpp"

namespace raytracer::graphics
{
    class Canva
    {
        public:
            Canva() = delete;
            explicit Canva(const std::size_t width, const std::size_t height)
                : _buffer(width, height) {}

            ~Canva() { this->_buffer.clear(); }

            void setPixelColor(const unsigned int x, const unsigned int y, const Color& color) { this->_buffer(x, y) = color; }
            [[nodiscard]] Color getPixelColor(const unsigned int x, const unsigned int y) const { return this->_buffer(x, y); }
            [[nodiscard]] const auto& getPixelMatrix() const { return this->_buffer; }
            [[nodiscard]] auto getWidth() const { return this->_buffer.getSizeX(); }
            [[nodiscard]] auto getHeight() const { return this->_buffer.getSizeY(); }

        protected:
            math::DynamicMatrix2D<Color> _buffer;

    };

}
