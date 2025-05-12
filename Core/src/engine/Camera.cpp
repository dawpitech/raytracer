/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Camera.cpp
*/

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>

#include "Camera.hpp"
#include "Canva.hpp"
#include "RACIST/IObject.hpp"
#include "RACIST/Ray.hpp"
#include "utils/ProgressBar.hpp"

#include <mutex>
#include <condition_variable>
#include <queue>

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

void raytracer::engine::Camera::renderThread(const Scene& scene, graphics::Canva& targetCanva, 
                                    int startX, int endX, int startY, int endY) const
{
    std::mt19937 rng(this->_rd());
    for (int j = startY; j < endY; j++) {
        for (int i = startX; i < endX; i++) {
            graphics::Color pixelColor(0, 0, 0);
            for (int sample = 0; sample < this->_sampleRate; sample++) {
                Ray ray = this->getRandomRay(i, j, rng);
                pixelColor += ray_color(ray, MAX_DEPTH, scene);
            }
            targetCanva.setPixelColor(i - startX, j - startY, 
                graphics::Color(this->_pixelSampleScale * pixelColor));
        }
    }
}

void raytracer::engine::Camera::render(const Scene& scene, const graphics::IRenderer& renderer) const
{
    const int tileSize = 32;
    const int imageWidth = this->_image_width;
    const int imageHeight = this->_image_height;
    int numThreads = std::thread::hardware_concurrency();

    if (numThreads <= 0) {
        throw std::invalid_argument("Number of threads must be positive");
    }

    std::clog << "[TRACE] Now rendering with " << numThreads << " threads..." << std::endl;

    std::queue<Tile> tiles;
    std::mutex queueMutex;

    for (int y = 0; y < imageHeight; y += tileSize) {
        for (int x = 0; x < imageWidth; x += tileSize) {
            int endX = std::min(x + tileSize, imageWidth);
            int endY = std::min(y + tileSize, imageHeight);
            tiles.push(Tile{x, endX, y, endY});
        }
    }

    const auto timeBefore = std::chrono::system_clock::now();

    std::vector<std::thread> threads;
    for (int t = 0; t < numThreads; ++t) {
        threads.emplace_back([this, &scene, &tiles, &queueMutex]() {
            std::mt19937 rng(this->_rd());

            while (true) {
                Tile tile;

                {
                    std::lock_guard<std::mutex> lock(queueMutex);
                    if (tiles.empty()) break;
                    tile = tiles.front();
                    tiles.pop();
                }

                for (int j = tile.startY; j < tile.endY; ++j) {
                    for (int i = tile.startX; i < tile.endX; ++i) {
                        graphics::Color pixelColor(0, 0, 0);
                        for (int sample = 0; sample < this->_sampleRate; ++sample) {
                            Ray ray = this->getRandomRay(i, j, rng);
                            pixelColor += ray_color(ray, MAX_DEPTH, scene);
                        }

                        this->_canva->setPixelColor(i, j, graphics::Color(this->_pixelSampleScale * pixelColor));
                    }
                }
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    auto timeEnd = std::chrono::system_clock::now();
    std::cout << "[INFO] Rendering took "
              << std::chrono::duration_cast<std::chrono::milliseconds>((timeEnd - timeBefore)).count()
              << "ms" << std::endl;

    renderer.renderCanva(*this->_canva);
}

raytracer::engine::Camera::Camera(const double aspect_ratio, const int image_width)
    : _aspect_ratio(aspect_ratio)
    , _fov(90)
    , _image_width(image_width)
    , _sampleRate(100)
    , _viewport_width(0)
    , _viewport_height(0)
    , _image_height(static_cast<int>(image_width / _aspect_ratio))
    , _pixelSampleScale(0)
    , _pixel_delta_u(0.f, 0.f, 0.f)
    , _pixel_delta_v(0.f, 0.f, 0.f)
    , _dist(0.0, 1.0)
{
    this->updateRenderingConfig();
}

void raytracer::engine::Camera::updateRenderingConfig()
{
    this->_image_height = static_cast<int>(_image_width / _aspect_ratio);
    if (this->_image_height < 1)
        throw CameraException();

    this->_center = this->_position;

    const auto theta = math::degrees_to_radians(this->_fov);
    const auto h = std::tan(theta/2);
    this->_viewport_height = 2 * h * FOCAL_LENGTH;
    this->_viewport_width = this->_viewport_height * (static_cast<double>(this->_image_width) / this->_image_height);
    const auto viewport_u = math::Vec3<double>(this->_viewport_width, 0, 0);
    const auto viewport_v = math::Vec3<double>(0, -this->_viewport_height, 0);

    this->_pixel_delta_u = viewport_u / this->_image_width;
    this->_pixel_delta_v = viewport_v / this->_image_height;

    const auto viewport_upper_left =
        _center - math::Vec3<double>(0, 0, FOCAL_LENGTH) - viewport_u / 2 - viewport_v / 2;
    this->_pixel00_location = math::Point3D{viewport_upper_left + 0.5 * (this->_pixel_delta_u + this->_pixel_delta_v)};

    this->_canva = std::make_unique<graphics::Canva>(this->_image_width, this->_image_height);
    this->_pixelSampleScale = 1.0 / this->_sampleRate;
}

raytracer::engine::Ray raytracer::engine::Camera::getRandomRay(const int i, const int j, std::mt19937 &rng) const
{
    const auto randomOffset = sampleSquare(rng);
    const auto pixelPos = this->_pixel00_location
        + ((i + randomOffset.x()) * this->_pixel_delta_u)
        + ((j + randomOffset.y()) * this->_pixel_delta_v);
    const auto rayOrigin = this->_center;
    const auto rayDirection = pixelPos - rayOrigin;

    return Ray{rayOrigin, rayDirection};
}

raytracer::math::Vec3<double> raytracer::engine::Camera::sampleSquare(std::mt19937 &rng) const
{
    return raytracer::math::Vec3<double>{this->_dist(rng) - 0.5, this->_dist(rng) - 0.5, 0};
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

void raytracer::engine::Camera::setFOV(const double fov)
{
    this->_fov = fov;
}

void raytracer::engine::Camera::setPosition(const math::Point3D& position)
{
    this->_position = position;
}
