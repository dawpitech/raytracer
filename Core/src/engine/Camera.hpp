/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Camera.hpp
*/

#pragma once

#include <random>

#include "Canva.hpp"
#include "Scene.hpp"
#include "WorldConfiguration.hpp"
#include "RACIST/Color.hpp"
#include "renderers/IRenderer.hpp"

namespace raytracer::engine
{
    class Camera
    {
        public:
            class CameraException : public std::exception {};

            Camera() = delete;
            explicit Camera(double aspect_ratio, int image_width);
            ~Camera() = default;

            void render(const WorldConfiguration& worldConfiguration, const Scene& scene, graphics::IRenderer& renderer, unsigned int threadCount) const;
            int renderNoThread(const WorldConfiguration& worldConfiguration, const Scene& scene,
                               graphics::IRenderer& renderer) const;
            void updateRenderingConfig();

            void setAspectRatio(double aspectRatio);
            void setFOV(double fov);
            void setImageWidth(int imageWidth);
            void setSampleRate(int sampleRate);
            void setPosition(const math::Point3D& position);
            void setRotation(const math::Vec3<double>& rotation);

        private:
            static constexpr double FOCAL_LENGTH = 1.0;
            static constexpr int MAX_DEPTH = 10;
            static constexpr int MULTI_THREADING_TILE_SIZE = 128;

            double _aspect_ratio;
            double _fov;
            int _image_width;
            int _sampleRate;
            math::Point3D _position;
            math::Vec3<double> _rotation;

            std::unique_ptr<graphics::Canva> _canva;
            double _viewport_width;
            double _viewport_height;
            int _image_height;
            double _pixelSampleScale;
            math::Point3D _center;
            math::Point3D _pixel00_location;
            math::Vec3<double> _pixel_delta_u;
            math::Vec3<double> _pixel_delta_v;

            mutable std::random_device _rd;
            mutable std::uniform_real_distribution<> _dist;

            [[nodiscard]] static graphics::Color ray_color(const WorldConfiguration& worldConfiguration, const Ray& ray, int depth, const Scene& scene);
            [[nodiscard]] Ray getRandomRay(int i, int j, std::mt19937 &rng) const;
            [[nodiscard]] math::Vec3<double> sampleSquare(std::mt19937 &rng) const;
            [[nodiscard]] std::tuple<math::Vec3<double>, math::Vec3<double>, math::Vec3<double>> computeCameraVectors() const;
            static math::Vec3<double> rotateVector(double cx, double sx,
                                                   double cy, double sy,
                                                   double cz, double sz,
                                                   const math::Vec3<double>& vec);
            static math::Vec3<double> normalize(const math::Vec3<double>& vec);
    };
}
