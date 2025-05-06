/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** main.cpp
*/

#include <iostream>

#include "Raytracer.hpp"

static void printHelp() { std::cout << "USAGE: ./raytracer <SCENE_FILE>\n\tSCENE_FILE: scene configuration" << std::endl; }

int main(const int argc, const char** argv)
{
    raytracer::Raytracer raytracer;

    if (argc != 2)
        return printHelp(), 84;
    const std::string arg(argv[1]);
    if (arg == "-help" || arg == "--help" || arg == "-h")
        return printHelp(), 0;

    raytracer.loadPlugins();
    raytracer.parseSceneConfig(arg);
    raytracer.run();
    return 0;
}
