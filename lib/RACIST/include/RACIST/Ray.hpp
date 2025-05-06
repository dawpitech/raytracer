/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Ray.hpp
*/

#pragma once

#include "MATH/Point.hpp"
#include "MATH/Vec3.hpp"

namespace raytracer::engine
{

    class Ray
    {
        public:
            Ray()
                : direction(0.f, 0.f, 0.f) {}
            Ray(const raytracer::math::Point3D& _origin, const math::Vec3<double>& _direction)
                : origin(_origin), direction(_direction) {}
            ~Ray() = default;

            [[nodiscard]] const math::Point3D& getOrigin() const { return this->origin; }
            [[nodiscard]] const math::Vec3<double>& getDirection() const { return this->direction; }

            [[nodiscard]] math::Point3D at(const double t) const { return math::Point3D{origin + t*direction}; }

        protected:
            math::Point3D origin;
            math::Vec3<double> direction;
    };
}
