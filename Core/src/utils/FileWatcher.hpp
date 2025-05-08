/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** FileWatcher.hpp
*/

#pragma once

#include <filesystem>

namespace raytracer::utils
{
    class FileWatcher
    {
        public:
            static std::filesystem::file_time_type getLastEditTime(const std::string& filepath)
            {
                return last_write_time(std::filesystem::path(filepath));
            }
    };
}
