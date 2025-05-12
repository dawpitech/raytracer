/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** main.cpp
*/

#include <iostream>

#include "Raytracer.hpp"

int main(const int argc, const char** argv)
{
    raytracer::Raytracer raytracer;

    const int rtValue = raytracer.parseArgs(argc, argv);
    if (rtValue == 1)
        return 0;
    if (rtValue != 0)
        return rtValue;
    raytracer.loadPlugins();
    if (raytracer.parseSceneConfig() != 0)
        return 84;
    raytracer.run();
    return 0;
}
