/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Matrix.hpp
*/

#pragma once

namespace raytracer::math
{
    template <typename K, size_t rows, size_t columns>
    class Matrix2D
    {
        public:
            Matrix2D() : _2DArray() {}
            ~Matrix2D() = default;

            K operator[](const int i) const { return this[i]; }
            K& operator[](const int i)
            {
                if (i < 0 || i >= columns)
                    throw std::out_of_range("Out of range access on a 2D Matrix");
                return *this->_2DArray.at(i);
            }

        private:
            std::array<std::array<K, rows>, columns> _2DArray;
    };
}
