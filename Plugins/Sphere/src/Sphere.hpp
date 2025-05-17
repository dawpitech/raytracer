/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Sphere.hpp
*/

#pragma once

#include <memory>

#include "RACIST/HitRecord.hpp"
#include "RACIST/IMaterial.hpp"
#include "RACIST/IObject.hpp"

namespace raytracer::engine::objects::sphere
{
    // ReSharper disable once CppClassCanBeFinal
    class Sphere : public IObject
    {
        public:
            class InvalidSphereRadiusException final : public ObjectException {};

            Sphere(const math::Point3D& center, double radius);
            ~Sphere() override = default;

            bool hit(const Ray& ray, const math::Interval& ray_t, HitRecord& record) const override;
            void setMaterial(std::unique_ptr<materials::IMaterial>& material) override;
            [[nodiscard]] AABB getBoundingDox() const override;

        protected:
            std::unique_ptr<materials::IMaterial> _material;
            math::Point3D _center;
            double _radius;
            AABB _axisAlignedBoundingBox;
    };
};
