/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Plane.cpp
*/

#include "Plane.hpp"

raytracer::engine::objects::plane::Plane::Plane(const math::Point3D& point, const math::Vec3<double>& normal)
    : _point(point), _normal(normal / normal.length()) {}

bool raytracer::engine::objects::plane::Plane::hit(const Ray& ray, const math::Interval& ray_t, HitRecord& record) const
{
    const double denom = math::Vec3<double>::dot(_normal, ray.getDirection());
    if (std::abs(denom) < 1e-8)
        return false;

    const double t = math::Vec3<double>::dot(_point - ray.getOrigin(), _normal) / denom;
    if (!ray_t.contains(t))
        return false;

    record.t = t;
    record.point = ray.at(t);
    record.normal = _normal;
    record.point_outward = denom < 0;
    record.normal = record.point_outward ? _normal : -_normal;
    record.material = _material.get();
    return true;
}

void raytracer::engine::objects::plane::Plane::setMaterial(std::unique_ptr<materials::IMaterial>& material)
{
    _material = std::move(material);
}

raytracer::engine::AABB raytracer::engine::objects::plane::Plane::getBoundingDox() const
{
    return AABB(
        math::Interval{-math::infinity, math::infinity},
        math::Interval{-math::infinity, math::infinity},
        math::Interval{-math::infinity, math::infinity}
    );
}
