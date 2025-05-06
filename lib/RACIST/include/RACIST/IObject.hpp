/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** IObject.hpp
*/

#pragma once

#include "Ray.hpp"
#include "MATH/Interval.hpp"

namespace raytracer::engine
{
    class HitRecord
    {
        public:
            HitRecord() : normal(0, 0, 0), t() {}

            math::Point3D point;
            math::Vec3<double> normal;
            double t;
            bool point_outward;
    };

    class IObject
    {
        public:
            class ObjectException : public std::exception {};

            IObject() = default;
            virtual ~IObject() = default;

            virtual bool hit(const Ray& ray, const math::Interval& ray_t, HitRecord& record) const = 0;
    };
}
