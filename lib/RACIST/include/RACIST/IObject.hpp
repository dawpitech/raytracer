/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** IObject.hpp
*/

#pragma once

#include <memory>

#include "AxisAlignBoundingBox.hpp"
#include "IMaterial.hpp"
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
            virtual void setMaterial(std::unique_ptr<materials::IMaterial>& material) = 0;
            [[nodiscard]] virtual AABB getBoundingDox() const = 0;
    };
}
