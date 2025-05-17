/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Vec3.hpp
*/

#pragma once

#include <cmath>
#include <ostream>

#include "Constants.hpp"

namespace raytracer::math
{
    template <typename K>
    class Vec3
    {
        public:
            Vec3(K x, K y, K z)
            {
                this->e[0] = x;
                this->e[1] = y;
                this->e[2] = z;
            }

            K x() const { return e[0]; }
            K y() const { return e[1]; }
            K z() const { return e[2]; }

            K& operator[](const int i)
            {
                if (i < 0 || i > 2)
                    throw std::out_of_range("Out of range access on Vector3");
                return e[i];
            }
            K operator[](const int i) const { return this->e[i]; }

            Vec3 operator-() const { return Vec3{-e[0], -e[1], -e[2]}; }
            Vec3& operator/=(const K value) { return *this *= 1/value; }
            Vec3& operator+=(const Vec3& other)
            {
                e[0] += other.x();
                e[1] += other.y();
                e[2] += other.z();
                return *this;
            }
            Vec3& operator+=(const K value)
            {
                e[0] += value;
                e[1] += value;
                e[2] += value;
                return *this;
            }

            Vec3& operator*=(const K value)
            {
                e[0] *= value;
                e[1] *= value;
                e[2] *= value;
                return *this;
            }

            [[nodiscard]] double length() const
            {
                return std::sqrt(this->length_squared());
            }
            [[nodiscard]] double length_squared() const
            {
                return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
            }
            [[nodiscard]] Vec3 unit_vector() const
            {
                return *this / this->length();
            }

            Vec3 operator*(const K value) const
            {
                return Vec3{e[0] * value, e[1] * value, e[2] * value};
            }

            [[nodiscard]] static Vec3<double> random_unit_vector()
            {
                while(true)
                {
                    const Vec3<double> p = Vec3::random(-1, 1);
                    if (const auto lensq = p.length_squared(); 1e-160 < lensq && lensq <= 1)
                        return p / sqrt(lensq);
                }
            }
            [[nodiscard]] static Vec3<double> random_on_hemisphere(const Vec3& normal)
            {
                Vec3<double> on_unit_sphere = random_unit_vector();
                if (Vec3::dot(on_unit_sphere, normal) > 0.0)
                    return on_unit_sphere;
                return -on_unit_sphere;
            }

            template <typename T>
            friend Vec3 operator/(const Vec3& v, const T& value)
            {
                return math::Vec3{v.x() / value, v.y() / value, v.z() / value};
            }

            static K dot(const Vec3& l, const Vec3& r)
            {
                return l.x() * r.x() + l.y() * r.y() + l.z() * r.z();
            }

            static Vec3 random()
            {
                // NOT YET IMPLEMENTED
                throw std::exception();
            }

            static Vec3 random(const K min, const K max)
            {
                // NOT YET IMPLEMENTED
                throw std::exception();
            }

            Vec3 operator+(const Vec3& r) const
            {
                const auto l = *this;
                return math::Vec3{l.x() + r.x(), l.y() + r.y(), l.z() + r.z()};
            }
            // ReSharper disable once CppMemberFunctionMayBeStatic
            [[nodiscard]] bool isNearZero() const
            {
                // NOT YET IMPLEMENTED
                throw std::exception();
            }

            [[nodiscard]] Vec3 reflect(const Vec3& normal) const
            {
                double factor = 2 * Vec3::dot(*this, normal);
                const Vec3 scaled_normal(normal.x() * factor, normal.y() * factor, normal.z() * factor);
                return Vec3(e[0] - scaled_normal.x(), e[1] - scaled_normal.y(), e[2] - scaled_normal.z());
            }

            [[nodiscard]] Vec3 refract(const Vec3& normal, const double etaiEtatRatio) const
            {
                const auto cosTheta = std::fmin(Vec3::dot(-(*this), normal), 1.0);
                const Vec3 rValuePerpendicular = etaiEtatRatio * (*this + cosTheta * normal);
                const Vec3 rValueParallel = - std::sqrt(std::fabs(1.0 - rValuePerpendicular.length_squared())) * normal;
                return rValuePerpendicular + rValueParallel;
            }

        protected:
            K e[3];
    };

    template <>
    inline Vec3<double> Vec3<double>::random()
    {
        return Vec3{random_double(), random_double(), random_double()};
    }

    template <>
    inline Vec3<double> Vec3<double>::random(const double min, const double max)
    {
        return Vec3{random_double(min, max), random_double(min, max), random_double(min, max)};
    }

    template <>
    inline bool Vec3<double>::isNearZero() const
    {
        static constexpr auto s = 1e-8;
        return std::fabs(this->e[0]) < s && std::fabs(this->e[1]) < s && std::fabs(this->e[2]) < s;
    }

    inline Vec3<double> operator*(const double value, const Vec3<double>& v)
    {
        return Vec3{v.x() * value, v.y() * value, v.z() * value};
    }

    template <typename T, typename K>
    Vec3<K> operator*(const T& value, const Vec3<K>& v)
    {
        return Vec3{v.x() * value, v.y() * value, v.z() * value};
    }

    template <typename K>
    Vec3<K> operator+(const Vec3<K>& l, const Vec3<K>& r)
    {
        return Vec3{l.x() + r.x(), l.y() + r.y(), l.z() + r.z()};
    }

    template <typename K>
    Vec3<K> operator-(const Vec3<K>& l, const Vec3<K>& r)
    {
        return Vec3{l.x() - r.x(), l.y() - r.y(), l.z() - r.z()};
    }

}

template <typename K>
std::ostream& operator<<(std::ostream& out, const raytracer::math::Vec3<K>& v)
{
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
}
