/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** main.cpp
*/

#include "Raytracer.hpp"

int main()
{
    raytracer::Raytracer raytracer;

    raytracer.loadPlugins();
    raytracer.parseSceneConfig("./scenes/default.cfg");
    raytracer.run();
    return 0;
}
