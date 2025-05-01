/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Camera.cpp
*/

#include <iostream>

#include "Camera.hpp"

raytracer::graphics::Color raytracer::engine::Camera::ray_color(const Ray& ray, const Scene& scene) const
{
    HitRecord hitRecord;
    if (scene.hit(ray, math::Interval{0, math::infinity}, hitRecord))
        return graphics::Color{0.5 * (hitRecord.normal + graphics::Color{1, 1, 1})};

    const auto unit_dir = ray.getDirection().unit_vector();
    const auto a = 0.5 * (unit_dir.y() + 1.0);
    return graphics::Color{(1.0 - a) * graphics::Color{1.0, 1.0, 1.0} + a * graphics::Color{0.5, 0.7, 1.0}};
}

void raytracer::engine::Camera::render(const Scene& scene, const graphics::IRenderer& renderer) const
{
    for (int j = 0; j < this->_image_height; j++) {
        std::cout << "\rLines remaining: " << this->_image_width - j << ' ' << std::flush;
        for (int i = 0; i < this->_image_width; i++) {
            const auto pixel_center = this->_pixel00_location +
                (i * this->_pixel_delta_u) + (j * this->_pixel_delta_v);
            const auto ray_direction = pixel_center - this->_center;

            const Ray ray{this->_center, ray_direction};
            const auto pixel_color = ray_color(ray, scene);

            this->_canva->setPixelColor(i, j, pixel_color);
        }
    }
    std::cout << "\rDone.                    " << std::endl;

    renderer.renderCanva(*this->_canva);
}

raytracer::engine::Camera::Camera(const double aspect_ratio, const int image_width)
    : _pixel_delta_u(0.f, 0.f, 0.f), _pixel_delta_v(0.f, 0.f, 0.f)
{
    _aspect_ratio = aspect_ratio;
    _image_width = image_width;

    _image_height = static_cast<int>(_image_width / _aspect_ratio);
    if (_image_height < 1)
        throw CameraException();

    _viewport_width = VIEWPORT_HEIGHT * (static_cast<double>(_image_width) / _image_height);
    const auto viewport_u = math::Vec3<double>(_viewport_width, 0, 0);
    const auto viewport_v = math::Vec3<double>(0, -VIEWPORT_HEIGHT, 0);

    _pixel_delta_u = viewport_u / _image_width;
    _pixel_delta_v = viewport_v / _image_height;

    const auto viewport_upper_left =
        _center - math::Vec3<double>(0, 0, FOCAL_LENGTH) - viewport_u / 2 - viewport_v / 2;
    _pixel00_location = math::Point3D{viewport_upper_left + 0.5 * (_pixel_delta_u + _pixel_delta_v)};

    this->_canva = std::make_unique<graphics::Canva>(this->_image_width, this->_image_height);
}
