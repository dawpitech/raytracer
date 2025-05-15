/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Diffuse.cpp
*/

#include <cassert>

#include "Diffuse.hpp"

raytracer::engine::materials::DirectionalLight::DirectionalLight(const int r, const int g, const int b)
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

raytracer::engine::materials::DirectionalLight::DirectionalLight(const graphics::Color& color)
    : _albedo(color)
{
}

bool raytracer::engine::materials::DirectionalLight::scatterRay(const Ray& rayIn, const HitRecord& hitRecord, graphics::Color& colorAttenuation, Ray& rayScattered)
{
    auto scatterDirection = hitRecord.normal + math::Vec3<double>::random_unit_vector();

    if (scatterDirection.isNearZero())
        scatterDirection = hitRecord.normal;

    rayScattered = Ray{hitRecord.point, scatterDirection};
    colorAttenuation = this->_albedo;
    return true;
}
