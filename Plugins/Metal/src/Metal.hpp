/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Metal.hpp
*/

#pragma once

#include "RACIST/HitRecord.hpp"
#include "RACIST/ANonEmissiveMaterial.hpp"

namespace raytracer::engine::materials
{
    // ReSharper disable once CppClassCanBeFinal
    class Metal : public ANonEmissiveMaterial
    {
        public:
            Metal(int r, int g, int b, double fuzz);
            explicit Metal(const graphics::Color& color, double fuzz);
            ~Metal() override = default;
        
            bool scatterRay(const Ray& rayIn, const HitRecord& hitRecord, graphics::Color& colorAttenuation, Ray& rayScattered) override;

        private:
            const graphics::Color _albedo;
            const double _fuzz;
    };
}
