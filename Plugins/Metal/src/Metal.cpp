/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Metal.cpp
*/

#include <cassert>

#include "Metal.hpp"

#include <iostream>

raytracer::engine::materials::Metal::Metal(const int r, const int g, const int b, const double fuzz)
    : _albedo(graphics::Color{
        static_cast<double>(r) / 255,
        static_cast<double>(g) / 255,
        static_cast<double>(b) / 255
        })
    , _fuzz(fuzz)
{
    assert(r >= 0 && r <= 255);
    assert(g >= 0 && r <= 255);
    assert(b >= 0 && r <= 255);
    assert(fuzz >= 0 && fuzz <= 1);
}

raytracer::engine::materials::Metal::Metal(const graphics::Color& color, const double fuzz)
    : _albedo(color)
    , _fuzz(fuzz)
{
    assert(fuzz >= 0 && fuzz <= 1);
}

bool raytracer::engine::materials::Metal::scatterRay(const Ray& rayIn, const HitRecord& hitRecord, graphics::Color& colorAttenuation, Ray& rayScattered)
{
    auto rayReflected = rayIn.getDirection().reflect(hitRecord.normal);
    rayReflected = rayReflected.unit_vector() + (this->_fuzz * math::Vec3<double>::random_unit_vector());

    rayScattered = Ray{hitRecord.point, rayReflected};
    colorAttenuation = this->_albedo;
    return math::Vec3<double>::dot(rayScattered.getDirection(), hitRecord.normal) > 0;
}
