/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PPMRenderer.cpp
*/

#include <fstream>
#include <ostream>

#include "PPMRenderer.hpp"

void raytracer::graphics::PPMRenderer::renderCanva(const Canva& canva) const
{
    std::ofstream output("output.ppm");

    output << "P3\n" << canva.getWidth() << ' ' << canva.getHeight() << "\n255\n";
    for (int j = 0; j < canva.getHeight(); j++)
        for (int i = 0; i < canva.getWidth(); i++)
            output
                << canva.getPixelColor(i, j).rScaled() << ' '
                << canva.getPixelColor(i, j).gScaled() << ' '
                << canva.getPixelColor(i, j).bScaled() << '\n';

    output.close();
}
