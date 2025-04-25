/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Matrix.hpp
*/

#pragma once

#include <array>
#include <cstddef>

namespace raytracer::math
{
    template <typename K, std::size_t sizeX, std::size_t sizeY>
    class Matrix2D
    {
        public:
            Matrix2D() = default;
            ~Matrix2D() = default;

            K& operator()(std::size_t x, std::size_t y)
            {
                if (x >= sizeX || y >= sizeY)
                    throw std::out_of_range("Matrix2D index out of bounds");
                return _2DArray[x][y];
            }

            const K& operator()(std::size_t x, std::size_t y) const
            {
                if (x >= sizeX || y >= sizeY)
                    throw std::out_of_range("Matrix2D index out of bounds");
                return _2DArray[x][y];
            }

        private:
            std::array<std::array<K, sizeY>, sizeX> _2DArray;
    };
}
