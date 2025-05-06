/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Sphere.hpp
*/

#pragma once

#include "RACIST/IObject.hpp"

namespace raytracer::engine::objects::sphere
{
    class Sphere : public IObject
    {
        public:
            class InvalidSphereRadiusException final : public ObjectException {};

            Sphere(const math::Point3D& center, double radius);

            bool hit(const Ray& ray, const math::Interval& ray_t, HitRecord& record) const override;

        protected:
            math::Point3D _center;
            double _radius;
    };
};
