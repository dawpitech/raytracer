/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PPMRenderer.cpp
*/

#include <fstream>
#include <ostream>

#include "PPMRenderer.hpp"
#include "../Canva.hpp"

void raytracer::graphics::PPMRenderer::renderCanva(const Canva& canva)
{
    std::ofstream output("output.ppm");

    output << "P3\n" << canva.getWidth() << ' ' << canva.getHeight() << "\n255\n";
    for (int j = 0; j < canva.getHeight(); j++)
        for (int i = 0; i < canva.getWidth(); i++)
            output
                << canva.getPixelColor(i, j).rGammaScaled() << ' '
                << canva.getPixelColor(i, j).gGammaScaled() << ' '
                << canva.getPixelColor(i, j).bGammaScaled() << '\n';

    output.close();
}
