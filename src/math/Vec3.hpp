/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Vec3.hpp
*/

#pragma once

#include <cmath>
#include <ostream>

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
            K operator[](const int i) const { return this[i]; }

            Vec3 operator-() const { return Vec3{-e[0], -e[1], -e[2]}; }
            Vec3& operator/=(const K value) { return *this *= 1/value; }
            Vec3& operator+=(const Vec3& other)
            {
                e[0] += other.x();
                e[1] += other.y();
                e[2] += other.z();
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

            template <typename T>
            friend Vec3 operator/(const Vec3& v, const T& value)
            {
                return math::Vec3{v.x() / value, v.y() / value, v.z() / value};
            }

            static K dot(const Vec3& l, const Vec3& r)
            {
                return l.x() * r.x() + l.y() * r.y() + l.z() * r.z();
            }

        protected:
            K e[3];
    };
}

template <typename K>
std::ostream& operator<<(std::ostream& out, const raytracer::math::Vec3<K>& v)
{
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

template <typename T, typename K>
raytracer::math::Vec3<K> operator*(const T& value, const raytracer::math::Vec3<K>& v)
{
    return raytracer::math::Vec3{v.x() * value, v.y() * value, v.z() * value};
}

template <typename K>
raytracer::math::Vec3<K> operator+(const raytracer::math::Vec3<K>& l, const raytracer::math::Vec3<K>& r)
{
    return raytracer::math::Vec3{l.x() + r.x(), l.y() + r.y(), l.z() + r.z()};
}

template <typename K>
raytracer::math::Vec3<K> operator-(const raytracer::math::Vec3<K>& l, const raytracer::math::Vec3<K>& r)
{
    return raytracer::math::Vec3{l.x() - r.x(), l.y() - r.y(), l.z() - r.z()};
}
