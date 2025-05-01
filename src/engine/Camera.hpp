/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Camera.hpp
*/

#pragma once

#include "engine/Canva.hpp"
#include "engine/Color.hpp"
#include "engine/Scene.hpp"
#include "engine/renderers/IRenderer.hpp"

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

        private:
            static constexpr double FOCAL_LENGTH = 1.0;
            static constexpr double VIEWPORT_HEIGHT = 2.0;

            std::unique_ptr<graphics::Canva> _canva;
            double _viewport_width;
            double _aspect_ratio;
            int _image_height;
            int _image_width;
            math::Point3D _center;
            math::Point3D _pixel00_location;
            math::Vec3<double> _pixel_delta_u;
            math::Vec3<double> _pixel_delta_v;

            [[nodiscard]] graphics::Color ray_color(const Ray& ray, const Scene& scene) const;
    };
}
