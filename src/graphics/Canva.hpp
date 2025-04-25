/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Canva.hpp
*/

#pragma once

#include "InternalConfig.hpp"
#include "graphics/Color.hpp"
#include "math/Matrix.hpp"

namespace raytracer::graphics
{
    class Canva
    {
        public:
            Canva() = default;
            ~Canva() = default;

            void setPixelColor(const unsigned int x, const unsigned int y, const Color& color)
            {
                this->_buffer(x, y) = color;
            }
            [[nodiscard]] Color getPixelColor(const unsigned int x, const unsigned int y) const
            {
                return this->_buffer(x, y);
            }
            [[nodiscard]] const auto& getPixelMatrix() const
            {
                return this->_buffer;
            }

        protected:
            math::Matrix2D<Color, IMAGE_WIDTH, IMAGE_WIDTH> _buffer;
    };

}
