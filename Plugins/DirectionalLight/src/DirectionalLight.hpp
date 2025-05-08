/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** DirectionalLight.hpp
*/

#pragma once

#include "RACIST/HitRecord.hpp"
#include "RACIST/IMaterial.hpp"

namespace raytracer::engine::materials
{
    // ReSharper disable once CppClassCanBeFinal
    class DirectionalLight : public IMaterial
    {
        public:
            DirectionalLight(int r, int g, int b, double intensity);
            explicit DirectionalLight(const graphics::Color& color, double intensity);
            ~DirectionalLight() override = default;
        
            bool scatterRay(const Ray& rayIn, const HitRecord& hitRecord, graphics::Color& colorAttenuation, Ray& rayScattered) override;
            graphics::Color emittedColor() override;

        private:
            const graphics::Color _albedo;
            const double _intensity;
    };
}
