/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SDLRenderer.cpp
*/

#include "SDLRenderer.hpp"

#include <iostream>

raytracer::graphics::SDLRenderer::SDLRenderer()
    : _window(nullptr)
    , _renderer(nullptr)
    , _texture(nullptr)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw Exception();

    this->_window = SDL_CreateWindow(
        "Raytracer",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        100,
        100,
        SDL_WINDOW_SHOWN
    );
    if (this->_window == nullptr)
        throw Exception();

    this->_renderer = SDL_CreateRenderer(this->_window, -1, SDL_RENDERER_ACCELERATED);
    if (this->_renderer == nullptr)
        throw Exception();
}

void raytracer::graphics::SDLRenderer::renderLine(const Canva& canva, const int y)
{
    if (y == 0) {
        SDL_SetWindowSize(this->_window, static_cast<int>(canva.getWidth()), static_cast<int>(canva.getHeight()));
        this->_texture = SDL_CreateTexture(
            this->_renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET,
            static_cast<int>(canva.getWidth()),
            static_cast<int>(canva.getHeight())
        );
        if (this->_texture == nullptr)
            throw Exception();
    }

    SDL_SetRenderTarget(this->_renderer, this->_texture);

    for (int x = 0; x < canva.getWidth(); x++) {
        SDL_SetRenderDrawColor(
            this->_renderer,
            canva.getPixelColor(x, y).rGammaScaled(),
            canva.getPixelColor(x, y).gGammaScaled(),
            canva.getPixelColor(x, y).bGammaScaled(),
            SDL_ALPHA_OPAQUE
        );
        SDL_RenderDrawPoint(this->_renderer, x, y);
    }

    SDL_SetRenderTarget(this->_renderer, nullptr);
    SDL_RenderCopy(this->_renderer, this->_texture, nullptr, nullptr);
    SDL_RenderPresent(this->_renderer);
}

void raytracer::graphics::SDLRenderer::renderCanva(const Canva& canva)
{
    if (this->_texture == nullptr) {
        SDL_SetWindowSize(this->_window, static_cast<int>(canva.getWidth()), static_cast<int>(canva.getHeight()));
        this->_texture = SDL_CreateTexture(
            this->_renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET,
            static_cast<int>(canva.getWidth()),
            static_cast<int>(canva.getHeight())
        );
        if (this->_texture == nullptr)
            throw Exception();
    }

    SDL_SetRenderTarget(this->_renderer, this->_texture);

    for (int x = 0; x < canva.getWidth(); x++) {
        for (int y = 0; y < canva.getHeight(); y++)
        {
            SDL_SetRenderDrawColor(
               this->_renderer,
               canva.getPixelColor(x, y).rGammaScaled(),
               canva.getPixelColor(x, y).gGammaScaled(),
               canva.getPixelColor(x, y).bGammaScaled(),
               SDL_ALPHA_OPAQUE
           );
            SDL_RenderDrawPoint(this->_renderer, x, y);
        }
    }

    SDL_SetRenderTarget(this->_renderer, nullptr);
    SDL_RenderCopy(this->_renderer, this->_texture, nullptr, nullptr);
    SDL_RenderPresent(this->_renderer);
}


bool raytracer::graphics::SDLRenderer::exitRequested() const
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            return true;
    }
    return false;
}
