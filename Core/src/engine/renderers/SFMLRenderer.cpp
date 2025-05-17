/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SFMLRenderer.cpp
*/

#include "SFMLRenderer.hpp"

#include <iostream>

raytracer::graphics::SFMLRenderer::SFMLRenderer()
    : _window(sf::VideoMode(1000, 1000), "Raytracer", sf::Style::None)
{
}

raytracer::graphics::SFMLRenderer::~SFMLRenderer()
{
    this->_window.close();
}

void raytracer::graphics::SFMLRenderer::renderLine(const Canva& canva, int y)
{
    if (y == 0) {
        this->_window.setSize({
            static_cast<unsigned>(canva.getWidth()),
            static_cast<unsigned>(canva.getHeight())
        });

        const sf::View view(sf::FloatRect(0, 0, static_cast<float>(canva.getWidth()), static_cast<float>(canva.getHeight())));
        this->_window.setView(view);
    }

    this->_window.clear();
    for (int realY = 0; realY <= y; realY++) {
        for (int x = 0; x < canva.getWidth(); x++) {
            sf::RectangleShape pixelShape(sf::Vector2f(1.0f, 1.0f));
            pixelShape.setPosition(static_cast<float>(x), static_cast<float>(realY));
            pixelShape.setFillColor(sf::Color{
                static_cast<unsigned char>(canva.getPixelColor(x, realY).rGammaScaled()),
                static_cast<unsigned char>(canva.getPixelColor(x, realY).gGammaScaled()),
                static_cast<unsigned char>(canva.getPixelColor(x, realY).bGammaScaled()),
            });

            this->_window.draw(pixelShape);
        }
    }

    this->_window.display();
}

void raytracer::graphics::SFMLRenderer::renderCanva(const Canva& canva)
{
    this->_window.setSize({
        static_cast<unsigned>(canva.getWidth()),
        static_cast<unsigned>(canva.getHeight())
    });

    const sf::View view(sf::FloatRect(0, 0, static_cast<float>(canva.getWidth()), static_cast<float>(canva.getHeight())));
    this->_window.setView(view);

    this->_window.clear();

    for (int x = 0; x < canva.getWidth(); x++) {
        for (int y = 0; y < canva.getHeight(); y++)
        {
            sf::RectangleShape pixelShape(sf::Vector2f(1.0f, 1.0f));
            pixelShape.setPosition(static_cast<float>(x), static_cast<float>(y));
            pixelShape.setFillColor(sf::Color{
                static_cast<unsigned char>(canva.getPixelColor(x, y).rGammaScaled()),
                static_cast<unsigned char>(canva.getPixelColor(x, y).gGammaScaled()),
                static_cast<unsigned char>(canva.getPixelColor(x, y).bGammaScaled()),
            });

            this->_window.draw(pixelShape);
        }
    }

    this->_window.display();
}

bool raytracer::graphics::SFMLRenderer::exitRequested()
{
    sf::Event event{};

    while (this->_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            return true;
    }
    return false;
}
