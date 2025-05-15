/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Cylinder.cpp
*/

#include "Cylinder.hpp"
#include <cmath>
#include <limits>

using namespace raytracer::engine::objects::cylinder;

Cylinder::Cylinder(const math::Point3D& baseCenter, double radius, double height)
    : _baseCenter(baseCenter), _radius(radius), _height(height) {}

bool Cylinder::hit(const Ray& ray, const math::Interval& ray_t, HitRecord& record) const
{
    const auto& origin = ray.getOrigin();
    const auto& dir = ray.getDirection();

    double dx = dir.x();
    double dz = dir.z();
    double ox = origin.x() - _baseCenter.x();
    double oz = origin.z() - _baseCenter.z();

    double a = dx * dx + dz * dz;
    double b = 2 * (ox * dx + oz * dz);
    double c = ox * ox + oz * oz - _radius * _radius;

    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return false;

    double sqrt_disc = std::sqrt(discriminant);
    double t1 = (-b - sqrt_disc) / (2 * a);
    double t2 = (-b + sqrt_disc) / (2 * a);

    bool hit_found = false;
    double t_closest = ray_t.max;
    HitRecord temp_record;

    for (double t : {t1, t2}) {
        if (!ray_t.contains(t))
            continue;

        math::Point3D p = ray.at(t);
        double y = p.y() - _baseCenter.y();

        if (_height != std::numeric_limits<double>::infinity()) {
            if (y < 0 || y > _height)
                continue;
        }

        math::Vec3<double> normal = math::Vec3<double>{p.x() - _baseCenter.x(), 0.0, p.z() - _baseCenter.z()};
		normal = normal / normal.length();

        temp_record.t = t;
        temp_record.point = p;
        temp_record.normal = normal;
        temp_record.point_outward = math::Vec3<double>::dot(dir, normal) < 0;
        if (!temp_record.point_outward)
            temp_record.normal = -normal;
        temp_record.material = _material.get();

        if (t < t_closest) {
            t_closest = t;
            record = temp_record;
            hit_found = true;
        }
    }

    if (_height != std::numeric_limits<double>::infinity()) {
        for (int i = 0; i < 2; ++i) {
            double y_plane = _baseCenter.y() + (i == 0 ? 0.0 : _height);
            double denom = dir.y();
            if (std::abs(denom) < 1e-8)
                continue;

            double t = (y_plane - origin.y()) / denom;
            if (!ray_t.contains(t) || t >= t_closest)
                continue;

            math::Point3D p = ray.at(t);
            double dx_disk = p.x() - _baseCenter.x();
            double dz_disk = p.z() - _baseCenter.z();
            if (dx_disk * dx_disk + dz_disk * dz_disk <= _radius * _radius) {
                temp_record.t = t;
                temp_record.point = p;
                temp_record.normal = math::Vec3{0.0, (i == 0 ? -1.0 : 1.0), 0.0};
                temp_record.point_outward = math::Vec3<double>::dot(dir, temp_record.normal) < 0;
                if (!temp_record.point_outward)
                    temp_record.normal = -temp_record.normal;
                temp_record.material = _material.get();

                t_closest = t;
                record = temp_record;
                hit_found = true;
            }
        }
    }

    return hit_found;
}

void Cylinder::setMaterial(std::unique_ptr<materials::IMaterial>& material)
{
    _material = std::move(material);
}
