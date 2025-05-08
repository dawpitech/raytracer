/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Diffuse.hpp
*/

#pragma once

#include "RACIST/HitRecord.hpp"
#include "RACIST/IMaterial.hpp"

namespace raytracer::engine::materials
{
    // ReSharper disable once CppClassCanBeFinal
    class Diffuse : public IMaterial
    {
        public:
            Diffuse(int r, int g, int b);
            explicit Diffuse(graphics::Color color);
            ~Diffuse() override = default;
        
            bool scatterRay(const Ray& rayIn, const HitRecord& hitRecord, graphics::Color& colorAttenuation, Ray& rayScattered) override;

        private:
            graphics::Color _albedo;
    };
}
