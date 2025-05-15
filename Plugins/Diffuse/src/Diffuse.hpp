/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Diffuse.hpp
*/

#pragma once

#include "RACIST/ANonEmissiveMaterial.hpp"
#include "RACIST/HitRecord.hpp"

namespace raytracer::engine::materials
{
    // ReSharper disable once CppClassCanBeFinal
    class Diffuse : public ANonEmissiveMaterial
    {
        public:
            Diffuse(int r, int g, int b);
            explicit Diffuse(const graphics::Color& color);
            ~Diffuse() override = default;
        
            bool scatterRay(const Ray& rayIn, const HitRecord& hitRecord, graphics::Color& colorAttenuation, Ray& rayScattered) override;

        private:
            const graphics::Color _albedo;
    };
}
