/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** constants.hpp
*/

#pragma once

#include <cstdlib>
#include <limits>
#include <random>

namespace raytracer::math
{
    const double infinity = std::numeric_limits<double>::infinity();
    const double pi = 3.1415926535897932385;
    static std::random_device _rd;
    static std::mt19937 _rng(_rd());
    static std::uniform_real_distribution<> dist(0.0, 1.0);

    inline double degrees_to_radians(double degrees) {
        return degrees * pi / 180.0;
    }

    inline double random_double()
    {
        return dist(_rng);
    }

    inline double random_double(const double min, const double max)
    {
        return min + (max - min) * random_double();
    }
}
