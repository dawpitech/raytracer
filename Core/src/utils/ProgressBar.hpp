/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ProgressBar.hpp
*/

#pragma once
#include <atomic>

namespace raytracer::utils
{
    class ProgressBar
    {
        public:
            ProgressBar() = default;
            ~ProgressBar() = default;

            static std::string render(const unsigned int size, const double advancement)
            {
                std::string bar;
                if (advancement > 1.0 || advancement < 0)
                    throw std::exception();

                for (unsigned int i = 0; i < size; i++)
                    bar += L'.';
                for (unsigned int i = 0; i < size * advancement; i++)
                    bar.at(i) = L'#';

                return bar;
            }
    };
}
