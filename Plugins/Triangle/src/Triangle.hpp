/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Triangle.hpp
*/

#pragma once

#include <memory>

#include "RACIST/HitRecord.hpp"
#include "RACIST/IMaterial.hpp"
#include "RACIST/IObject.hpp"

namespace raytracer::engine::objects::triangle
{
    class Triangle : public IObject
    {
        public:
            Triangle(const math::Point3D& v1, const math::Point3D& v2, const math::Point3D& v3);
            ~Triangle() override = default;

            bool hit(const Ray& ray, const math::Interval& ray_t, HitRecord& record) const override;
            void setMaterial(std::unique_ptr<materials::IMaterial>& material) override;
            [[nodiscard]] AABB getBoundingDox() const override;

        protected:
            std::unique_ptr<materials::IMaterial> _material;
            math::Point3D _v1;
            math::Point3D _v2;
            math::Point3D _v3;
            math::Vec3<double> _normal;
            
            void computeNormal();
    };
}
