/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Diffuse.cpp
*/

#include "Diffuse.hpp"

raytracer::engine::materials::Diffuse::Diffuse(const int r, const int g, const int b)
    : _albedo(graphics::Color{
        static_cast<double>(r) * 255,
        static_cast<double>(g) * 255,
        static_cast<double>(b) * 255
        })
{
}

raytracer::engine::materials::Diffuse::Diffuse(graphics::Color color)
    : _albedo(color)
{
}

bool raytracer::engine::materials::Diffuse::scatterRay(const Ray& rayIn, const HitRecord& hitRecord, graphics::Color& colorAttenuation, Ray& rayScattered)
{
    const auto scatterDirection = hitRecord.normal + math::Vec3<double>::random_unit_vector();
    rayScattered = Ray{hitRecord.point, scatterDirection};
    colorAttenuation = this->_albedo;
    return true;
}
