/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Cone.cpp
*/

#include "Cone.hpp"
#include <cmath>
#include <limits>

raytracer::engine::objects::cone::Cone::Cone(const math::Point3D& apex, const math::Vec3<double>& direction, double angle_degrees, double height)
 	: _apex(apex), _direction(direction), _height(height)
{
    double angle_radians = angle_degrees * (M_PI / 180.0);
    _tan_angle = std::tan(angle_radians);

    double len = _direction.length();
    if (len != 0.0)
        _direction = _direction / len;

    const double inf = std::numeric_limits<double>::infinity();
    constexpr double eps = 1e-8;

    math::Interval ix, iy, iz;
    double dx = _direction.x();
    double dy = _direction.y();
    double dz = _direction.z();

    double base_radius = (_height == inf)
        ? inf
        : _height * _tan_angle;

    if (_height == inf) {
        ix = (std::abs(dx) < eps)
                ? math::Interval{_apex.x() - base_radius, _apex.x() + base_radius}
                : math::Interval{-inf, +inf};
        iy = (std::abs(dy) < eps)
                ? math::Interval{_apex.y() - base_radius, _apex.y() + base_radius}
                : math::Interval{-inf, +inf};
        iz = (std::abs(dz) < eps)
                ? math::Interval{_apex.z() - base_radius, _apex.z() + base_radius}
                : math::Interval{-inf, +inf};
    } else {
        math::Point3D base_center(
            _apex.x() + dx * _height,
            _apex.y() + dy * _height,
            _apex.z() + dz * _height
        );

        double rx = base_radius * std::sqrt(std::max(0.0, 1.0 - dx*dx));
        double ry = base_radius * std::sqrt(std::max(0.0, 1.0 - dy*dy));
        double rz = base_radius * std::sqrt(std::max(0.0, 1.0 - dz*dz));

        double minX = std::min(_apex.x(), base_center.x()) - rx;
        double maxX = std::max(_apex.x(), base_center.x()) + rx;
        ix = math::Interval{minX, maxX};

        double minY = std::min(_apex.y(), base_center.y()) - ry;
        double maxY = std::max(_apex.y(), base_center.y()) + ry;
        iy = math::Interval{minY, maxY};

        double minZ = std::min(_apex.z(), base_center.z()) - rz;
        double maxZ = std::max(_apex.z(), base_center.z()) + rz;
        iz = math::Interval{minZ, maxZ};
    }

    _axisAlignedBoundingBox = AABB(ix, iy, iz);
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
    bool hit_found = false;
    double t_closest = ray_t.max;
    HitRecord temp_record;

    if (discriminant >= 0) {
        double sqrt_discriminant = std::sqrt(discriminant);
        double t1 = (-b - sqrt_discriminant) / (2.0 * a);
        double t2 = (-b + sqrt_discriminant) / (2.0 * a);

        for (double t : {t1, t2}) {
            if (!ray_t.contains(t))
                continue;

            math::Point3D intersection_point = ray.at(t);
            math::Vec3<double> intersection_vec = intersection_point - _apex;
            double projection = math::Vec3<double>::dot(intersection_vec, _direction);

            if (projection < 0 || projection > _height)
                continue;

            math::Vec3<double> axis_proj = _direction * projection;
            math::Vec3<double> radial = intersection_vec - axis_proj;
            math::Vec3<double> normal = radial - _direction * (k * radial.length());

            temp_record.t = t;
            temp_record.point = intersection_point;
            temp_record.normal =  normal / normal.length();
            temp_record.point_outward = math::Vec3<double>::dot(D, temp_record.normal) < 0;
            if (!temp_record.point_outward)
                temp_record.normal = -temp_record.normal;

            temp_record.material = _material.get();

            if (t < t_closest) {
                t_closest = t;
                record = temp_record;
                hit_found = true;
            }
        }
    }

    if (_height != std::numeric_limits<double>::infinity()) {
        math::Point3D base_center = raytracer::math::Point3D(
            _apex.x() + _direction.x() * _height,
            _apex.y() + _direction.y() * _height,
            _apex.z() + _direction.z() * _height
        );
        double denom = math::Vec3<double>::dot(D, _direction);
    
        if (std::abs(denom) > 1e-8) {
            double t_disk = math::Vec3<double>::dot(base_center - ray.getOrigin(), _direction) / denom;
            if (ray_t.contains(t_disk)) {
                math::Point3D p = ray.at(t_disk);
                math::Vec3<double> v = p - base_center;
                double radius = _height * _tan_angle;
    
                if (v.length_squared() <= radius * radius && t_disk < t_closest) {
                    temp_record.t = t_disk;
                    temp_record.point = p;
                    temp_record.normal = _direction;
                    temp_record.point_outward = math::Vec3<double>::dot(D, _direction) < 0;
                    if (!temp_record.point_outward)
                        temp_record.normal = -temp_record.normal;
    
                    temp_record.material = _material.get();
    
                    t_closest = t_disk;
                    record = temp_record;
                    hit_found = true;
                }
            }
        }
    }    

    return hit_found;
}

void raytracer::engine::objects::cone::Cone::setMaterial(std::unique_ptr<materials::IMaterial>& material)
{
  	_material = std::move(material);
}

raytracer::engine::AABB raytracer::engine::objects::cone::Cone::getBoundingDox() const
{
	return this->_axisAlignedBoundingBox;
}
