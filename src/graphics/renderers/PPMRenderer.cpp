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

    output << "P3\n" << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << "\n255\n";
    for (int j = 0; j < IMAGE_HEIGHT; j++)
        for (int i = 0; i < IMAGE_WIDTH; i++)
            output << canva.getPixelColor(i, j).rScaled() << ' ' << canva.getPixelColor(i, j).gScaled() << ' ' << canva.getPixelColor(i, j).bScaled() << '\n';

    output.close();
}
