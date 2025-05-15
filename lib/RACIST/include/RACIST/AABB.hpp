/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** AABB.hpp
*/

#pragma once

#include "MATH/Interval.hpp"

namespace raytracer::engine
{
    class AABB
    {
        public:
            AABB() = default;
            ~AABB() = default;

            AABB(const math::Interval& x, const math::Interval& y, const math::Interval& z)
                : _x(x), _y(y), _z(z) {}

            AABB(const math::Point3D& a, const math::Point3D& b)
            {
                const auto getIntervalFromDoublePoints = [](const math::Point3D& i, const math::Point3D& j, const int idx)
                {
                    if (idx < 0 || idx > 2)
                        throw std::range_error("Cannot retrieve data from Point at idx < 0 or idx > 2");
                    return i[idx] <= j[idx] ? math::Interval{i[idx], j[idx]} : math::Interval{j[idx], i[idx]};
                };

                this->_x = getIntervalFromDoublePoints(a, b, 0);
                this->_y = getIntervalFromDoublePoints(a, b, 1);
                this->_z = getIntervalFromDoublePoints(a, b, 2);
            }

            [[nodiscard]] const math::Interval& axis_interval(const int n) const
            {
                switch (n)
                {
                    case 0: return this->_x;
                    case 1: return this->_y;
                    case 2: return this->_z;
                    default: throw std::range_error("Cannot retrieve Interval from AABB at idx < 0 or idx > 2");
                }
            }

            [[nodiscard]] bool hit(const Ray& r, math::Interval tRay) const
            {
                const auto& rayOrigin = r.getOrigin();
                const auto& rayDirection = r.getDirection();

                for (int n = 0; n < 3; n++) {
                    const math::Interval& axis = this->axis_interval(n);
                    const double axisDirectionInverse = 1.0 / rayDirection[n];

                    auto t0 = (axis.min - rayOrigin[n]) * axisDirectionInverse;
                    auto t1 = (axis.max - rayOrigin[n]) * axisDirectionInverse;

                    if (t0 > t1)
                        std::swap(t0, t1);

                    if (t0 > tRay.min)
                        tRay.min = t0;
                    if (t1 < tRay.max)
                        tRay.max = t1;

                    if (tRay.max <= tRay.min)
                        return false;
                }
                return true;
            }

        protected:
            math::Interval _x;
            math::Interval _y;
            math::Interval _z;
    };
}
