/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Color.hpp
*/

#pragma once

#include "math/Vec3.hpp"

namespace raytracer::graphics
{
    class Color : public math::Vec3<double>
    {
        public:
            Color()
                : Vec3(0.f, 0.f, 0.f) {}
            Color(const double _r, const double _g, const double _b)
                : Vec3(_r, _g, _b) {}
            explicit Color(const Vec3& v)
                : Vec3(v) {}
            ~Color() = default;

            [[nodiscard]] double x() const = delete;
            [[nodiscard]] double y() const = delete;
            [[nodiscard]] double z() const = delete;

            [[nodiscard]] double r() const { return this->e[0]; }
            [[nodiscard]] double g() const { return this->e[1]; }
            [[nodiscard]] double b() const { return this->e[2]; }

            double& r() { return this->e[0]; }
            double& g() { return this->e[1]; }
            double& b() { return this->e[2]; }

            [[nodiscard]] int rScaled() const { return static_cast<int>(255.999 * this->r()); }
            [[nodiscard]] int gScaled() const { return static_cast<int>(255.999 * this->g()); }
            [[nodiscard]] int bScaled() const { return static_cast<int>(255.999 * this->b()); }
    };
}

inline std::ostream& operator<<(std::ostream& out, const raytracer::graphics::Color& color)
{
    return out << color.rScaled() << ' ' << color.gScaled() << ' ' << color.bScaled();
}
