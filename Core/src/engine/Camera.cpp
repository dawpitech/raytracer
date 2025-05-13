/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Camera.cpp
*/

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <random>
#include <thread>

#include "Camera.hpp"
#include "Canva.hpp"
#include "WorldConfiguration.hpp"
#include "RACIST/IObject.hpp"
#include "RACIST/Ray.hpp"
#include "utils/ProgressBar.hpp"

raytracer::graphics::Color raytracer::engine::Camera::ray_color( // NOLINT(*-no-recursion)
    const WorldConfiguration& worldConfiguration,
    const Ray& ray, const int depth, const Scene& scene)
{
    if (depth <= 0)
        return graphics::Color{0, 0, 0};

    HitRecord hitRecord;

    if (!scene.hit(ray, math::Interval{0.001, math::infinity}, hitRecord))
    {
        if (depth == MAX_DEPTH) {
            if (worldConfiguration.skybox.enabled) {
                const auto a = 0.5 * (ray.getDirection().unit_vector().y() + 1.0);
                return graphics::Color{(1.0 - a) * graphics::Color{1.0, 1.0, 1.0} + a * graphics::Color{0.5, 0.7, 1.0}};
            } else {
                return graphics::Color{0, 0, 0};
            }
        }
        if (worldConfiguration.skybox.enabled && worldConfiguration.skybox.illuminate) {
            const auto a = 0.5 * (ray.getDirection().unit_vector().y() + 1.0);
            return graphics::Color{(1.0 - a) * graphics::Color{1.0, 1.0, 1.0} + a * graphics::Color{0.5, 0.7, 1.0}};
        } else if (worldConfiguration.ambientLight.enabled) {
            return graphics::Color{worldConfiguration.ambientLight.color * worldConfiguration.ambientLight.intensity};
        }
        return graphics::Color{0, 0, 0};
    }

    // ReSharper disable CppTooWideScopeInitStatement
    Ray scatteredRay;
    graphics::Color attenuation;
    const graphics::Color materialEmittedColor = hitRecord.material->emittedColor();

    if (!hitRecord.material->scatterRay(ray, hitRecord, attenuation, scatteredRay))
        return materialEmittedColor;

    return graphics::Color{ray_color(worldConfiguration, scatteredRay, depth - 1, scene) * attenuation + materialEmittedColor};
}

void raytracer::engine::Camera::renderNoThread(const WorldConfiguration& worldConfiguration, const Scene& scene, graphics::IRenderer& renderer) const
{
    std::mt19937 rng(this->_rd());

    std::clog << "[TRACE] Now rendering..." << std::endl;
    const std::chrono::time_point<std::chrono::system_clock> timeBefore = std::chrono::system_clock::now();
    for (int j = 0; j < this->_image_height; j++) {
        const int progressBar = static_cast<int>(static_cast<double>(j) / this->_image_height * 100);
        for (int i = 0; i < this->_image_width; i++) {
            graphics::Color pixelColor(0, 0, 0);

            for (int sample = 0; sample < this->_sampleRate; sample++) {
                Ray ray = this->getRandomRay(i, j, rng);
                pixelColor += ray_color(worldConfiguration, ray, MAX_DEPTH, scene);
            }
            this->_canva->setPixelColor(i, j, graphics::Color(this->_pixelSampleScale * pixelColor));
        }
        if (j == 0) {
            const auto timeAfter = std::chrono::system_clock::now();
            // TODO: Improve algo, current * 2 is just gross approximation
            std::cout << "[INFO] Estimated rendering time: "
                << std::chrono::duration_cast<std::chrono::milliseconds>((timeAfter - timeBefore) * this->_image_height).count() * 2
                << "ms" << std::endl;
        }
        std::cout << "\r[INFO] RENDERING: " << utils::ProgressBar::render(10, static_cast<double>(progressBar) / 100) << " " << progressBar << "%" << std::flush;
        if (renderer.isInteractive())
            renderer.renderLine(*this->_canva, j);
    }
    const std::chrono::time_point<std::chrono::system_clock> timeEnd = std::chrono::system_clock::now();
    std::cout << "\r[INFO] RENDERING: Done                      " << std::endl;
    std::cout << "[INFO] Render took "
        << std::chrono::duration_cast<std::chrono::milliseconds>((timeEnd - timeBefore)).count()
        << "ms" << std::endl;
    if (!renderer.isInteractive())
        renderer.renderCanva(*this->_canva);
}

void raytracer::engine::Camera::render(const WorldConfiguration& worldConfiguration, const Scene& scene, graphics::IRenderer& renderer) const
{
    constexpr int tileSize = 128;
    const int imageWidth = this->_image_width;
    const int imageHeight = this->_image_height;
    const unsigned int numThreads = std::thread::hardware_concurrency();

    if (numThreads <= 0) {
        throw std::invalid_argument("Number of threads must be positive");
    }

    std::clog << "[TRACE] Now rendering with " << numThreads << " threads..." << std::endl;

    std::queue<graphics::IRenderer::Tile> tiles;
    std::mutex queueMutex;

    for (int y = 0; y < imageHeight; y += tileSize) {
        for (int x = 0; x < imageWidth; x += tileSize) {
            const int endX = std::min(x + tileSize, imageWidth);
            const int endY = std::min(y + tileSize, imageHeight);
            tiles.push(graphics::IRenderer::Tile{x, endX, y, endY});
        }
    }

    for (int y = 0; y < imageHeight; y++)
        for (int x = 0; x < imageWidth; x++)
            this->_canva->setPixelColor(x, y, graphics::Color{});

    const auto timeBefore = std::chrono::system_clock::now();

    std::vector<std::thread> threads;
    for (int t = 0; t < numThreads; ++t) {
        threads.emplace_back([this, &scene, &tiles, &queueMutex, worldConfiguration, &renderer] {
            std::mt19937 rng(this->_rd());

            while (true) {
                graphics::IRenderer::Tile tile{};

                {
                    std::lock_guard lock(queueMutex);
                    if (tiles.empty()) break;
                    tile = tiles.front();
                    tiles.pop();
                }

                for (int j = tile.startY; j < tile.endY; ++j) {
                    for (int i = tile.startX; i < tile.endX; ++i) {
                        graphics::Color pixelColor(0, 0, 0);
                        for (int sample = 0; sample < this->_sampleRate; ++sample) {
                            Ray ray = this->getRandomRay(i, j, rng);
                            pixelColor += ray_color(worldConfiguration, ray, MAX_DEPTH, scene);
                        }

                        this->_canva->setPixelColor(i, j, graphics::Color(this->_pixelSampleScale * pixelColor));
                    }
                }
            }
        });
    }

    for (auto& thread : threads)
        thread.join();

    const auto timeEnd = std::chrono::system_clock::now();
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
    , _rotation(0, 0, 0)
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

    auto [u, v, w] = this->computeCameraVectors();

    const auto viewport_u = u * this->_viewport_width;
    const auto viewport_v = -v * this->_viewport_height;

    this->_pixel_delta_u = viewport_u / this->_image_width;
    this->_pixel_delta_v = viewport_v / this->_image_height;

    const auto viewport_upper_left = _center - w * FOCAL_LENGTH - viewport_u / 2 - viewport_v / 2;
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

void raytracer::engine::Camera::setRotation(const math::Vec3<double>& rotation)
{
    this->_rotation = math::Vec3{rotation.x(), rotation.y(), rotation.z()};
}

std::tuple<raytracer::math::Vec3<double>, raytracer::math::Vec3<double>, raytracer::math::Vec3<double>>
    raytracer::engine::Camera::computeCameraVectors() const
{
    const auto xRad = math::degrees_to_radians(this->_rotation.x());
    const auto yRad = math::degrees_to_radians(this->_rotation.y());
    const auto zRad = math::degrees_to_radians(this->_rotation.z());

    const double cx = cos(xRad), sx = sin(xRad);
    const double cy = cos(yRad), sy = sin(yRad);
    const double cz = cos(zRad), sz = sin(zRad);

    const auto w =
        rotateVector(cx, sx, cy, sy, cz, sz,
            math::Vec3<double>{0, 0, 1}); // Forward
    const auto u =
        rotateVector(cx, sx, cy, sy, cz, sz,
            math::Vec3<double>{1, 0, 0}); // Right
    const auto v =
        rotateVector(cx, sx, cy, sy, cz, sz,
            math::Vec3<double>{0, 1, 0}); // IUp

    return {normalize(u), normalize(v), normalize(w)};
}

raytracer::math::Vec3<double> raytracer::engine::Camera::normalize(const math::Vec3<double>& vec)
{
    const double length = std::sqrt(vec.x() * vec.x() + vec.y() * vec.y() + vec.z() * vec.z());
    return math::Vec3{vec.x() / length, vec.y() / length, vec.z() / length};
}


raytracer::math::Vec3<double> raytracer::engine::Camera::rotateVector(
    const double cx, const double sx,
    const double cy, const double sy,
    const double cz, const double sz,
    const math::Vec3<double>& vec)
{
    const double x1 = cy * vec.x() + sy * vec.z();
    const double y1 = vec.y();
    const double z1 = -sy * vec.x() + cy * vec.z();

    const double x2 = x1;
    const double y2 = cx * y1 - sx * z1;
    const double z2 = sx * y1 + cx * z1;

    const double x3 = cz * x2 - sz * y2;
    const double y3 = sz * x2 + cz * y2;
    const double z3 = z2;

    return math::Vec3{x3, y3, z3};
}
