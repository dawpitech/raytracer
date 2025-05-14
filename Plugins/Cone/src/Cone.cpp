/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Cone.cpp
*/

#include "Cone.hpp"
#include <cmath>
#include <limits>

raytracer::engine::objects::cone::Cone::Cone(const math::Point3D& apex, const math::Vec3<double>& direction,
                                             double angle_degrees, double height)
    : _apex(apex), _direction(direction), _height(height)
{
    double angle_radians = angle_degrees * (M_PI / 180.0);

    _tan_angle = std::tan(angle_radians);

    double len = _direction.length();
    if (len != 0.0)
        _direction = _direction / len;
}

bool raytracer::engine::objects::cone::Cone::hit(const Ray& ray, const math::Interval& ray_t, HitRecord& record) const
{
    math::Vec3<double> CO = ray.getOrigin() - _apex;
    math::Vec3<double> D = ray.getDirection();
    math::Vec3<double> A = _direction;
    double k = _tan_angle;

    double d_dot_a = math::Vec3<double>::dot(D, A);
    double co_dot_a = math::Vec3<double>::dot(CO, A);

    double a = math::Vec3<double>::dot(D, D) - (1 + k * k) * d_dot_a * d_dot_a;
    double b = 2 * (math::Vec3<double>::dot(D, CO) - (1 + k * k) * d_dot_a * co_dot_a);
    double c = math::Vec3<double>::dot(CO, CO) - (1 + k * k) * co_dot_a * co_dot_a;

    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return false;

    double sqrt_discriminant = std::sqrt(discriminant);
    double t1 = (-b - sqrt_discriminant) / (2.0 * a);
    double t2 = (-b + sqrt_discriminant) / (2.0 * a);

    double t = t1;
    if (!ray_t.contains(t))
        t = t2;
    if (!ray_t.contains(t))
        return false;

    math::Point3D intersection_point = ray.at(t);
    math::Vec3<double> intersection_vec = intersection_point - _apex;
    double projection = math::Vec3<double>::dot(intersection_point - _apex, _direction);

    if (projection < 0 || projection > _height)
        return false;

    math::Vec3<double> axis_proj = _direction * math::Vec3<double>::dot(intersection_vec, _direction);
    math::Vec3<double> radial = intersection_vec - axis_proj;
    math::Vec3<double> normal = radial - _direction * (k * radial.length());

    record.t = t;
    record.point = intersection_point;
    record.normal = normal / normal.length();
    record.point_outward = math::Vec3<double>::dot(D, record.normal) < 0;
    if (!record.point_outward)
        record.normal = -record.normal;

    record.material = _material.get();
    return true;
}

void raytracer::engine::objects::cone::Cone::setMaterial(std::unique_ptr<materials::IMaterial>& material)
{
    _material = std::move(material);
}
