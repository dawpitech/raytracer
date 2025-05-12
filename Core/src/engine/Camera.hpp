/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Camera.hpp
*/

#pragma once

#include "Canva.hpp"
#include "Scene.hpp"
#include "RACIST/Color.hpp"
#include "RACIST/HitRecord.hpp"
#include "renderers/IRenderer.hpp"
#include <random>


namespace raytracer::engine
{
    class Camera
    {
        public:
            class CameraException : public std::exception {};

            Camera() = delete;
            explicit Camera(double aspect_ratio, int image_width);
            ~Camera() = default;

            void render(const Scene& scene, const graphics::IRenderer& renderer) const;
            void renderThread(const Scene& scene, graphics::Canva& targetCanva, 
                              int startX, int endX, int startY, int endY) const;
            void updateRenderingConfig();

            void setAspectRatio(double aspectRatio);
            void setFOV(double fov);
            void setImageWidth(int imageWidth);
            void setSampleRate(int sampleRate);
            void setPosition(const math::Point3D& position);

        private:
            static constexpr double FOCAL_LENGTH = 1.0;
            static constexpr int MAX_DEPTH = 10;

	    struct Tile {
		int startX, endX, startY, endY;
	    };

            double _aspect_ratio;
            double _fov;
            int _image_width;
            int _sampleRate;
            math::Point3D _position;

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

            [[nodiscard]] static graphics::Color ray_color(const Ray& ray, int depth, const Scene& scene);
            [[nodiscard]] Ray getRandomRay(int i, int j, std::mt19937 &rng) const;
            [[nodiscard]] math::Vec3<double> sampleSquare(std::mt19937 &rng) const;
    };
}
