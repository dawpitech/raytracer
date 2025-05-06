/*
** EPITECH PROJECT, 2025
** paradigm-day11
** File description:
** SafeDirectoryLister.hpp
*/

#ifndef SAFEDIRECTORYLISTER_HPP
    #define SAFEDIRECTORYLISTER_HPP

    #include <dirent.h>
    #include <string>

class SafeDirectoryLister
{
    public:
        class OpenFailureException final : public std::exception {};
        class NoMoreFileException final : public std::exception {};

        explicit SafeDirectoryLister() = default;
        SafeDirectoryLister(const std::string& path, bool hidden);
        ~SafeDirectoryLister();
        bool open(const std::string& path, bool hidden);
        [[nodiscard]] std::string get() const;

protected:
    DIR* _dir = nullptr;
    bool _showHiddenFiles = false;
};
#endif //SAFEDIRECTORYLISTER_HPP
