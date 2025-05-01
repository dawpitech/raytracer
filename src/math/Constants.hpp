/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** constants.hpp
*/

#pragma once

namespace raytracer::math
{
    const double infinity = std::numeric_limits<double>::infinity();
    const double pi = 3.1415926535897932385;

    inline double degrees_to_radians(double degrees) {
        return degrees * pi / 180.0;
    }

    inline double random_double()
    {
        return std::rand() / (RAND_MAX + 1.0);
    }

    inline double random_double(const double min, const double max)
    {
        return min + (max - min) * random_double();
    }
}
