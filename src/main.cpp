/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** main.cpp
*/

#include <thread>

#include "engine/Camera.hpp"
#include "engine/Scene.hpp"
#include "engine/objects/IObject.hpp"
#include "engine/objects/Sphere.hpp"
#include "engine/renderers/PPMRenderer.hpp"

int main()
{
    const raytracer::engine::Camera camera(16.0 / 9.0, 400);

    const std::unique_ptr<raytracer::graphics::IRenderer> renderer =
        std::make_unique<raytracer::graphics::PPMRenderer>();

    raytracer::engine::Scene world;

    std::unique_ptr<raytracer::engine::IObject> sphere =
        std::make_unique<raytracer::engine::objects::Sphere>(raytracer::math::Point3D{0, 0, -1}, 0.5);
    world.add(sphere);

    std::unique_ptr<raytracer::engine::IObject> sphere2 =
    std::make_unique<raytracer::engine::objects::Sphere>(raytracer::math::Point3D{0, -100.5, -1}, 100);
    world.add(sphere2);

    camera.render(world, *renderer);

    do {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    } while (!renderer->shouldExit());
    return 0;
}
