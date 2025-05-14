/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Cone.hpp
*/

#pragma once

#include <memory>

#include "RACIST/HitRecord.hpp"
#include "RACIST/IMaterial.hpp"
#include "RACIST/IObject.hpp"

namespace raytracer::engine::objects::cone
{
	class Cone : public IObject
	{
		public:
			Cone(const math::Point3D& apex, const math::Vec3<double>& direction, double angle, double height = INFINITY);
			~Cone() override = default;

			bool hit(const Ray& ray, const math::Interval& ray_t, HitRecord& record) const override;
			void setMaterial(std::unique_ptr<materials::IMaterial>& material) override;

			protected:
			std::unique_ptr<materials::IMaterial> _material;
			math::Point3D _apex;
			math::Vec3<double> _direction;
			double _angle;
			double _height;
			double _tan_angle;
			double _cos_theta;
	};
}
