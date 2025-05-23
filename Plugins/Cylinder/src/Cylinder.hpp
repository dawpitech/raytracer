/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Cylinder.hpp
*/

#pragma once

#include <memory>

#include "RACIST/HitRecord.hpp"
#include "RACIST/IMaterial.hpp"
#include "RACIST/IObject.hpp"

namespace raytracer::engine::objects::cylinder
{
	class Cylinder : public IObject
	{
		public:
			Cylinder(const math::Point3D& base, const math::Vec3<double>& direction, double radius, double height = INFINITY);
			~Cylinder() override = default;

			bool hit(const Ray& ray, const math::Interval& ray_t, HitRecord& record) const override;
			void setMaterial(std::unique_ptr<materials::IMaterial>& material) override;
			[[nodiscard]] AABB getBoundingDox() const override;

		protected:
			std::unique_ptr<materials::IMaterial> _material;
			math::Point3D _base;
			math::Vec3<double> _direction;
			double _radius;
			double _height;
			AABB _axisAlignedBoundingBox;
	};
}
