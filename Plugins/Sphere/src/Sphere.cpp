/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Sphere.cpp
*/

#include "Sphere.hpp"

raytracer::engine::objects::sphere::Sphere::Sphere(const math::Point3D& center, const double radius)
    : _center(center), _radius(radius)
{
    if (this->_radius < 0)
        throw InvalidSphereRadiusException();
}

bool raytracer::engine::objects::sphere::Sphere::hit(const Ray& ray, const math::Interval& ray_t, HitRecord& record) const
{
    const math::Vec3<double> oc = this->_center - ray.getOrigin();
    const auto a = ray.getDirection().length_squared();
    const auto h = math::Vec3<double>::dot(ray.getDirection(), oc);
    const auto c = oc.length_squared() - this->_radius * this->_radius;

    const auto discriminant = (h * h) - (a * c);
    if (discriminant < 0)
        return false;

    const auto sqrtd = std::sqrt(discriminant);

    auto root = (h - sqrtd) / a;
    if (!ray_t.surrounds(root)) {
        root = (h + sqrtd) / a;
        if (!ray_t.surrounds(root))
            return false;
    }

    record.t = root;
    record.point = ray.at(record.t);
    record.normal = (record.point - this->_center) / this->_radius;
    record.point_outward = math::Vec3<double>::dot(ray.getDirection(), record.normal) < 0;
    record.normal = record.point_outward ? record.normal : -record.normal;
    record.material = this->_material.get();
    return true;
}
