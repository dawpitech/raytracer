/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** DebugDiffuse.hpp
*/

#pragma once

#include "RACIST/ANonEmissiveMaterial.hpp"
#include "RACIST/HitRecord.hpp"

namespace raytracer::engine::materials
{
    // ReSharper disable once CppClassCanBeFinal
    class DebugDiffuse : public ANonEmissiveMaterial
    {
        public:
            DebugDiffuse(int r, int g, int b);
            explicit DebugDiffuse(const graphics::Color& color);
            ~DebugDiffuse() override = default;
        
            bool scatterRay(const Ray& rayIn, const HitRecord& hitRecord, graphics::Color& colorAttenuation, Ray& rayScattered) override;

        private:
            const graphics::Color _albedo;
    };
}
