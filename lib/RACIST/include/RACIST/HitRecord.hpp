/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** HitRecord.hpp
*/

#pragma once

//#include "IMaterial.hpp"
#include "MATH/Point.hpp"

namespace raytracer::engine::materials
{
    class IMaterial;
}

namespace raytracer::engine
{
    class HitRecord
    {
        public:
        HitRecord() : normal(0, 0, 0), t(), point_outward(), material() {}

        math::Point3D point;
        math::Vec3<double> normal;
        double t;
        bool point_outward;
        materials::IMaterial* material;
    };
}
