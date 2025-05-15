/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Cylinder.cpp
*/

#include "Cylinder.hpp"
#include <cmath>
#include <limits>

raytracer::engine::objects::cylinder::Cylinder::Cylinder(const math::Point3D& base, const math::Vec3<double>& direction, double radius, double height)
    : _base(base), _radius(radius), _height(height), _direction(direction)
{
    double len = _direction.length();
    if (len != 0.0)
        _direction = _direction / len;
}

bool raytracer::engine::objects::cylinder::Cylinder::hit(const Ray& ray, const math::Interval& ray_t, HitRecord& record) const
{
	math::Vec3<double> CO = ray.getOrigin() - _base;
	math::Vec3<double> D = ray.getDirection();
	math::Vec3<double> A = _direction;

	double d_dot_a = math::Vec3<double>::dot(D, A);
	double co_dot_a = math::Vec3<double>::dot(CO, A);

	math::Vec3<double> D_perp = D - A * d_dot_a;
	math::Vec3<double> CO_perp = CO - A * co_dot_a;

	double a = D_perp.length_squared();
	double b = 2 * math::Vec3<double>::dot(D_perp, CO_perp);
	double c = CO_perp.length_squared() - _radius * _radius;

	double discriminant = b * b - 4 * a * c;
	bool hit_found = false;
	double t_closest = ray_t.max;
	HitRecord temp_record;

	if (discriminant >= 0) {
		double sqrt_disc = std::sqrt(discriminant);
		double t1 = (-b - sqrt_disc) / (2.0 * a);
		double t2 = (-b + sqrt_disc) / (2.0 * a);

		for (double t : {t1, t2}) {
			if (!ray_t.contains(t))
				continue;

			math::Point3D p = ray.at(t);
			math::Vec3<double> v = p - _base;
			double proj = math::Vec3<double>::dot(v, A);

			if (proj < 0 || proj > _height)
				continue;

			math::Vec3<double> center_axis_point = _base + A * proj;
			math::Vec3<double> normal = (p - center_axis_point);
			normal = normal / normal.length();


			temp_record.t = t;
			temp_record.point = p;
			temp_record.normal = normal;
			temp_record.point_outward = math::Vec3<double>::dot(D, normal) < 0;
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
		for (int i = 0; i < 2; ++i) {
			math::Vec3<double> offset = A * (i == 0 ? 0.0 : _height);
			math::Point3D cap_center(_base.x() + offset.x(), _base.y() + offset.y(), _base.z() + offset.z());

			double denom = math::Vec3<double>::dot(D, A);

			if (std::abs(denom) > 1e-8) {
				double t_disk = math::Vec3<double>::dot(cap_center - ray.getOrigin(), A) / denom;
				if (ray_t.contains(t_disk)) {
					math::Point3D p = ray.at(t_disk);
					if ((p - cap_center).length_squared() <= _radius * _radius && t_disk < t_closest) {
						temp_record.t = t_disk;
						temp_record.point = p;
						temp_record.normal = A * (i == 0 ? -1.0 : 1.0);
						temp_record.point_outward = math::Vec3<double>::dot(D, temp_record.normal) < 0;
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
	}

	return hit_found;
}

void raytracer::engine::objects::cylinder::Cylinder::setMaterial(std::unique_ptr<materials::IMaterial>& material)
{
	_material = std::move(material);
}

raytracer::engine::AABB raytracer::engine::objects::cylinder::Cylinder::getBoundingDox() const
{
	return AABB(
		math::Interval{-math::infinity, math::infinity},
		math::Interval{-math::infinity, math::infinity},
		math::Interval{-math::infinity, math::infinity}
	);
}
