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

    template <typename K>
    class DynamicMatrix2D
    {
        public:
            DynamicMatrix2D() = delete;
            explicit DynamicMatrix2D(const std::size_t sizeX, const std::size_t sizeY)
                : _sizeX(sizeX), _sizeY(sizeY)
            {
                _2DArray.resize(sizeX, std::vector<K>(sizeY));
            }

            K& operator()(std::size_t x, std::size_t y)
            {
                if (x >= _sizeX || y >= _sizeY)
                    throw std::out_of_range("Matrix2D index out of bounds");
                return _2DArray[x][y];
            }

            const K& operator()(std::size_t x, std::size_t y) const
            {
                if (x >= _sizeX || y >= _sizeY)
                    throw std::out_of_range("Matrix2D index out of bounds");
                return _2DArray[x][y];
            }

        private:
            std::vector<std::vector<K>> _2DArray;
            std::size_t _sizeX;
            std::size_t _sizeY;
    };
}
