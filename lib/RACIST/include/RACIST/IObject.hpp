/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** IObject.hpp
*/

#pragma once

//#include "HitRecord.hpp"
#include "Ray.hpp"
#include "MATH/Interval.hpp"

namespace raytracer::engine
{
    class HitRecord;
}

namespace raytracer::engine
{
    class IObject
    {
        public:
            class ObjectException : public std::exception {};

            IObject() = default;
            virtual ~IObject() = default;

            virtual bool hit(const Ray& ray, const math::Interval& ray_t, HitRecord& record) const = 0;
    };
}
