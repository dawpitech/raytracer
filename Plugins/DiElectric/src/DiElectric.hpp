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
    class DiElectric : public ANonEmissiveMaterial
    {
        public:
            explicit DiElectric(double reflectionIndex, bool perfect);
            ~DiElectric() override = default;
        
            bool scatterRay(const Ray& rayIn, const HitRecord& hitRecord, graphics::Color& colorAttenuation, Ray& rayScattered) override;

        private:
            const double _reflectionIndex;
            const bool _perfect;
            [[nodiscard]] double reflectance(double cosine) const;
    };
}
