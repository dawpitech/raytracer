/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** IMaterial.hpp
*/

#pragma once

//#include "IObject.hpp"
#include "Ray.hpp"
#include "RACIST/Color.hpp"

namespace raytracer::engine
{
    class HitRecord;
}

namespace raytracer::engine::materials
{
    class IMaterial
    {
        public:
            class MaterialException : public std::exception {};

            IMaterial() = default;
            virtual ~IMaterial() = default;

            virtual bool scatterRay(const Ray& rayIn,
                const HitRecord& hitRecord,
                graphics::Color& colorAttenuation,
                Ray& rayScattered) = 0;
    };
}
