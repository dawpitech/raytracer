/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Diffuse.cpp
*/

#include <cassert>

#include "DiElectric.hpp"

raytracer::engine::materials::DiElectric::DiElectric(const double reflectionIndex, const bool perfect)
    : _reflectionIndex(reflectionIndex), _perfect(perfect)
{
}

bool raytracer::engine::materials::DiElectric::scatterRay(const Ray& rayIn, const HitRecord& hitRecord, graphics::Color& colorAttenuation, Ray& rayScattered)
{
    colorAttenuation = graphics::Color{1.0, 1.0, 1.0};
    const double RI = hitRecord.point_outward ? (1.0 / this->_reflectionIndex) : this->_reflectionIndex;

    const auto unitDirection = rayIn.getDirection().unit_vector();
    const auto cosTheta = std::fmin(math::Vec3<double>::dot(-unitDirection, hitRecord.normal), 1.0);
    const auto sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

    if (RI * sinTheta <= 1.0 || (this->_perfect ? 1.1 : this->reflectance(cosTheta)) > math::random_double())
        rayScattered = Ray{hitRecord.point, unitDirection.refract(hitRecord.normal, RI)};
    else
        rayScattered = Ray{hitRecord.point, unitDirection.reflect(hitRecord.normal)};
    return true;
}

[[nodiscard]] double raytracer::engine::materials::DiElectric::reflectance(const double cosine) const
{
    auto r0 = (1 - this->_reflectionIndex) / (1 + this->_reflectionIndex);
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow(1 - cosine, 5);
}
