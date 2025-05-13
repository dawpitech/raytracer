/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Triangle.cpp
*/

#include "Triangle.hpp"

raytracer::engine::objects::triangle::Triangle::Triangle(const math::Point3D& v1, const math::Point3D& v2, const math::Point3D& v3)
    : _v1(v1), _v2(v2), _v3(v3), _normal(0.0, 0.0, 0.0)
{
    computeNormal();
}

void raytracer::engine::objects::triangle::Triangle::computeNormal()
{
    math::Vec3<double> edge1 = _v2 - _v1;
    math::Vec3<double> edge2 = _v3 - _v1;
    
    _normal = math::Vec3<double>(
        edge1.y() * edge2.z() - edge1.z() * edge2.y(),
        edge1.z() * edge2.x() - edge1.x() * edge2.z(),
        edge1.x() * edge2.y() - edge1.y() * edge2.x()
    );
    
    double length = _normal.length();
    if (length > 0) {
        _normal = _normal / length;
    }
}

bool raytracer::engine::objects::triangle::Triangle::hit(const Ray& ray, const math::Interval& ray_t, HitRecord& record) const
{
    const math::Vec3<double> edge1 = _v2 - _v1;
    const math::Vec3<double> edge2 = _v3 - _v1;
    
    const math::Vec3<double> h(
        ray.getDirection().y() * edge2.z() - ray.getDirection().z() * edge2.y(),
        ray.getDirection().z() * edge2.x() - ray.getDirection().x() * edge2.z(),
        ray.getDirection().x() * edge2.y() - ray.getDirection().y() * edge2.x()
    );
    
    const double a = edge1.x() * h.x() + edge1.y() * h.y() + edge1.z() * h.z();
    
    if (std::abs(a) < 1e-8)
        return false;
    
    const double f = 1.0 / a;
    const math::Vec3<double> s = ray.getOrigin() - _v1;
    
    const double u = f * (s.x() * h.x() + s.y() * h.y() + s.z() * h.z());
    
    if (u < 0.0 || u > 1.0)
        return false;
    
    const math::Vec3<double> q(
        s.y() * edge1.z() - s.z() * edge1.y(),
        s.z() * edge1.x() - s.x() * edge1.z(),
        s.x() * edge1.y() - s.y() * edge1.x()
    );
    
    const double v = f * (ray.getDirection().x() * q.x() + 
                         ray.getDirection().y() * q.y() + 
                         ray.getDirection().z() * q.z());
    
    if (v < 0.0 || u + v > 1.0)
        return false;
    
    const double t = f * (edge2.x() * q.x() + edge2.y() * q.y() + edge2.z() * q.z());
    
    if (!ray_t.contains(t))
        return false;
    
    record.t = t;
    record.point = ray.at(t);
    record.normal = _normal;
    double dot_product = ray.getDirection().x() * _normal.x() + 
                        ray.getDirection().y() * _normal.y() + 
                        ray.getDirection().z() * _normal.z();
    record.point_outward = dot_product < 0;
    record.normal = record.point_outward ? _normal : -_normal;
    record.material = _material.get();
    
    return true;
}

void raytracer::engine::objects::triangle::Triangle::setMaterial(std::unique_ptr<materials::IMaterial>& material)
{
    _material = std::move(material);
}
