/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** DirectionalLight.cpp
*/

#include <cassert>

#include "DirectionalLight.hpp"

raytracer::engine::materials::DirectionalLight::DirectionalLight(const int r, const int g, const int b, const double intensity)
    : _albedo(graphics::Color{
        static_cast<double>(r) / 255,
        static_cast<double>(g) / 255,
        static_cast<double>(b) / 255
        })
    , _intensity(intensity)
{
    assert(r >= 0 && r <= 255);
    assert(g >= 0 && r <= 255);
    assert(b >= 0 && r <= 255);
    assert(_intensity >= 0);
}

raytracer::engine::materials::DirectionalLight::DirectionalLight(const graphics::Color& color, const double intensity)
    : _albedo(color)
    , _intensity(intensity)
{
    assert(_intensity >= 0);
}

bool raytracer::engine::materials::DirectionalLight::scatterRay(const Ray& rayIn, const HitRecord& hitRecord, graphics::Color& colorAttenuation, Ray& rayScattered)
{
    return false;
}

raytracer::graphics::Color raytracer::engine::materials::DirectionalLight::emittedColor()
{
    return graphics::Color{this->_albedo * this->_intensity};
}
