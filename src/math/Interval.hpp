/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Interval.hpp
*/

#pragma once

#include "Constants.hpp"

namespace raytracer::math
{
    class Interval {
        public:
            Interval() : min(+infinity), max(-infinity) {}
            Interval(const double min, const double max) : min(min), max(max) {}
            ~Interval() = default;

            [[nodiscard]] double size() const { return max - min; }
            [[nodiscard]] bool contains(const double x) const { return min <= x && x <= max; }
            [[nodiscard]] bool surrounds(const double x) const { return min < x && x < max; }

            static const Interval empty;
            static const Interval universe;

            double min;
            double max;
    };
}
