/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Camera.cpp
*/

#include <iostream>

#include "Camera.hpp"

#include <chrono>

#include "RACIST/IObject.hpp"
#include "RACIST/Ray.hpp"

raytracer::graphics::Color raytracer::engine::Camera::ray_color(const Ray& ray, const int depth, const Scene& scene) // NOLINT(*-no-recursion)
{
    if (depth <= 0)
        return graphics::Color{};

    // ReSharper disable CppTooWideScopeInitStatement
    if (HitRecord hitRecord; scene.hit(ray, math::Interval{0.001, math::infinity}, hitRecord)) {
        Ray scatteredRay;
        graphics::Color attenuation;
        if (hitRecord.material->scatterRay(ray, hitRecord, attenuation, scatteredRay))
            return graphics::Color{ray_color(scatteredRay, depth - 1, scene) * attenuation};

        return graphics::Color{0, 0, 0};
    }

    const auto unit_dir = ray.getDirection().unit_vector();
    const auto a = 0.5 * (unit_dir.y() + 1.0);
    return graphics::Color{(1.0 - a) * graphics::Color{1.0, 1.0, 1.0} + a * graphics::Color{0.5, 0.7, 1.0}};
}

void raytracer::engine::Camera::render(const Scene& scene, const graphics::IRenderer& renderer) const
{
    std::clog << "[TRACE] Now rendering..." << std::endl;
    const std::chrono::time_point<std::chrono::system_clock> timeBefore = std::chrono::system_clock::now();
    for (int j = 0; j < this->_image_height; j++) {
        const int progressBar = static_cast<int>(static_cast<double>(j) / this->_image_height * 100);
        for (int i = 0; i < this->_image_width; i++) {
            graphics::Color pixelColor(0, 0, 0);

            for (int sample = 0; sample < this->_sampleRate; sample++) {
                Ray ray = this->getRandomRay(i, j);
                pixelColor += ray_color(ray, MAX_DEPTH, scene);
            }
            this->_canva->setPixelColor(i, j, graphics::Color(this->_pixelSampleScale * pixelColor));
        }
        if (j == 0) {
            const auto timeAfter = std::chrono::system_clock::now();
            std::cout << "[INFO] Estimated rendering time: "
                << std::chrono::duration_cast<std::chrono::milliseconds>((timeAfter - timeBefore) * this->_image_height).count()
                << "ms" << std::endl;
        }
        std::cout << "\r[INFO] RENDERING: " << progressBar << "%" << std::flush;
    }
    const std::chrono::time_point<std::chrono::system_clock> timeEnd = std::chrono::system_clock::now();
    std::cout << "\r[INFO] RENDERING: Done.           " << std::endl;
    std::cout << "[INFO] Render took "
        << std::chrono::duration_cast<std::chrono::milliseconds>((timeEnd - timeBefore)).count()
        << "ms" << std::endl;

    renderer.renderCanva(*this->_canva);
}

raytracer::engine::Camera::Camera(const double aspect_ratio, const int image_width)
    : _viewport_width(VIEWPORT_HEIGHT * (static_cast<double>(image_width) / static_cast<int>(image_width / aspect_ratio)))
    , _aspect_ratio(aspect_ratio)
    , _image_height(static_cast<int>(image_width / _aspect_ratio))
    , _image_width(image_width)
    , _sampleRate(100)
    , _pixelSampleScale(0)
    , _pixel_delta_u(0.f, 0.f, 0.f)
    , _pixel_delta_v(0.f, 0.f, 0.f)
{
    this->updateRenderingConfig();
}

void raytracer::engine::Camera::updateRenderingConfig()
{
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
    this->_pixelSampleScale = 1.0 / this->_sampleRate;
}

raytracer::engine::Ray raytracer::engine::Camera::getRandomRay(const int i, const int j) const
{
    const auto randomOffset = sampleSquare();
    const auto pixelPos = this->_pixel00_location
        + ((i + randomOffset.x()) * this->_pixel_delta_u)
        + ((j + randomOffset.y()) * this->_pixel_delta_v);
    const auto rayOrigin = this->_center;
    const auto rayDirection = pixelPos - rayOrigin;

    return Ray{rayOrigin, rayDirection};
}

raytracer::math::Vec3<double> raytracer::engine::Camera::sampleSquare()
{
    return raytracer::math::Vec3<double>{math::random_double() - 0.5, math::random_double() - 0.5, 0};
}

void raytracer::engine::Camera::setAspectRatio(const double aspectRatio)
{
    this->_aspect_ratio = aspectRatio;
}

void raytracer::engine::Camera::setImageWidth(const int imageWidth)
{
    this->_image_width = imageWidth;
}

void raytracer::engine::Camera::setSampleRate(const int sampleRate)
{
    this->_sampleRate = sampleRate;
}
