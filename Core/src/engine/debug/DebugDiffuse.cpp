/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** DebugDiffuse.cpp
*/

#include <cassert>

#include "DebugDiffuse.hpp"

raytracer::engine::materials::DebugDiffuse::DebugDiffuse(const int r, const int g, const int b)
    : _albedo(graphics::Color{
        static_cast<double>(r) / 255,
        static_cast<double>(g) / 255,
        static_cast<double>(b) / 255
        })
{
    assert(r >= 0 && r <= 255);
    assert(g >= 0 && r <= 255);
    assert(b >= 0 && r <= 255);
}

raytracer::engine::materials::DebugDiffuse::DebugDiffuse(const graphics::Color& color)
    : _albedo(color)
{
}

bool raytracer::engine::materials::DebugDiffuse::scatterRay(const Ray& rayIn, const HitRecord& hitRecord, graphics::Color& colorAttenuation, Ray& rayScattered)
{
    auto scatterDirection = hitRecord.normal + math::Vec3<double>::random_unit_vector();

    if (scatterDirection.isNearZero())
        scatterDirection = hitRecord.normal;

    rayScattered = Ray{hitRecord.point, scatterDirection};
    colorAttenuation = this->_albedo;
    return true;
}
