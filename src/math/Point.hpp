/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Point.hpp
*/

#pragma once

#include "math/Vec3.hpp"

namespace raytracer::math
{
    class Point3D : public Vec3<double>
    {
        public:
            Point3D()
                : Vec3(0.f, 0.f, 0.f) {}
            explicit Point3D(const double x, const double y, const double z)
                : Vec3(x, y, z) {}
            explicit Point3D(const Vec3& v)
                : Vec3(v) {}
    };
}
