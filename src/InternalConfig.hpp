/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** InternalConfig.hpp
*/

#pragma once

static constexpr auto ASPECT_RATIO = 16.0 / 9.0;
static constexpr auto IMAGE_WIDTH = 400;
static constexpr auto IMAGE_HEIGHT = static_cast<int>(IMAGE_WIDTH / ASPECT_RATIO);

// Content bellow this line shouldn't be changed //
static_assert(IMAGE_HEIGHT >= 1);
