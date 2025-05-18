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
    const auto radiusVec = math::Vec3(radius, radius, radius);
    this->_axisAlignedBoundingBox = AABB(math::Point3D(center - radiusVec), math::Point3D(center + radiusVec));
}

/**
 * @brief Hit detection method, should influence the hitRecord if hitting
 * @details Return either or not the given ray hit the object, if so it should
 *          change the HitRecord to reflect what material was it, at what angle
 *          and at which point.
 * @param ray The ingoing ray
 * @param ray_t The Interval of valid values for the ray hits
 * @param record HitRecord giving information on the hit and how to compute
 *              its color.
 */
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

void raytracer::engine::objects::sphere::Sphere::setMaterial(std::unique_ptr<materials::IMaterial>& material)
{
    this->_material = std::move(material);
}

raytracer::engine::AABB raytracer::engine::objects::sphere::Sphere::getBoundingDox() const
{
    return this->_axisAlignedBoundingBox;
}
