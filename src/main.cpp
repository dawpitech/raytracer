/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** main.cpp
*/

#include <iostream>
#include <thread>

#include "InternalConfig.hpp"
#include "engine/Ray.hpp"
#include "engine/Scene.hpp"
#include "engine/objects/Sphere.hpp"
#include "graphics/Canva.hpp"
#include "graphics/Color.hpp"
#include "graphics/renderers/PPMRenderer.hpp"
#include "math/Constants.hpp"
#include "math/Interval.hpp"
#include "math/Point.hpp"

raytracer::graphics::Color ray_color(const raytracer::engine::Ray& ray, const raytracer::engine::Scene& world)
{
    raytracer::engine::HitRecord hitRecord;
    if (world.hit(ray, raytracer::math::Interval{0, raytracer::math::infinity}, hitRecord))
        return raytracer::graphics::Color{0.5 * (hitRecord.normal + raytracer::graphics::Color{1, 1, 1})};

    const auto unit_dir = ray.getDirection().unit_vector();
    const auto a = 0.5 * (unit_dir.y() + 1.0);
    return raytracer::graphics::Color{(1.0 - a) * raytracer::graphics::Color{1.0, 1.0, 1.0} + a * raytracer::graphics::Color{0.5, 0.7, 1.0}};
}

int main()
{
    constexpr auto focal_length = 1.0;
    constexpr auto viewport_height = 2.0;
    const double viewport_width = viewport_height * IMAGE_WIDTH / IMAGE_HEIGHT;
    const auto camera_center = raytracer::math::Point3D{};

    const auto viewport_u = raytracer::math::Vec3<double>{viewport_width, 0, 0};
    const auto viewport_v = raytracer::math::Vec3<double>{0, -viewport_height, 0};

    const auto viewport_du = viewport_u / IMAGE_WIDTH;
    const auto viewport_dv = viewport_v / IMAGE_HEIGHT;

    const auto viewport_upper_left_corner = camera_center - raytracer::math::Vec3<double>{0, 0, focal_length} - viewport_u / 2 - viewport_v / 2;
    const auto pixel_upper_left_coords = viewport_upper_left_corner + 0.5 * (viewport_du + viewport_dv);

    raytracer::graphics::Canva canva;

    const std::unique_ptr<raytracer::graphics::IRenderer> renderer =
        std::make_unique<raytracer::graphics::PPMRenderer>();

    raytracer::engine::Scene world;

    std::unique_ptr<raytracer::engine::IObject> sphere =
        std::make_unique<raytracer::engine::objects::Sphere>(raytracer::math::Point3D{0, 0, -1}, 0.5);
    world.add(sphere);

    std::unique_ptr<raytracer::engine::IObject> sphere2 =
    std::make_unique<raytracer::engine::objects::Sphere>(raytracer::math::Point3D{0, -100.5, -1}, 100);
    world.add(sphere2);

    for (int j = 0; j < IMAGE_HEIGHT; j++) {
        std::cout << "\rLines remaining: " << IMAGE_WIDTH - j << ' ' << std::flush;
        for (int i = 0; i < IMAGE_WIDTH; i++) {
            const auto pixel_center = pixel_upper_left_coords + (i * viewport_du) + (j * viewport_dv);
            const auto ray_direction = pixel_center - camera_center;

            const raytracer::engine::Ray ray{camera_center, ray_direction};
            const auto pixel_color = ray_color(ray, world);

            canva.setPixelColor(i, j, pixel_color);
        }
    }
    std::cout << "\rDone.                    " << std::endl;

    renderer->renderCanva(canva);

    while(true)
    {
        if (renderer->shouldExit())
            break;
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
   }
}
