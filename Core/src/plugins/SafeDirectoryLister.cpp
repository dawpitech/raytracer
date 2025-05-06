/*
** EPITECH PROJECT, 2025
** paradigm-day11
** File description:
** SafeDirectoryLister.cpp
*/

#include "SafeDirectoryLister.hpp"

SafeDirectoryLister::~SafeDirectoryLister()
{
    if (this->_dir != nullptr && closedir(this->_dir) == -1) { perror(nullptr); }
}

SafeDirectoryLister::SafeDirectoryLister(const std::string& path, const bool hidden)
{
    this->open(path, hidden);
}

bool SafeDirectoryLister::open(const std::string& path, const bool hidden)
{
    if (this->_dir != nullptr && closedir(this->_dir) == -1) { perror(nullptr); }
    DIR* dir = opendir(path.c_str());
    if (dir == nullptr) {
        this->_dir = nullptr;
        throw OpenFailureException();
    }
    this->_dir = dir;
    this->_showHiddenFiles = hidden;
    return true;
}

[[nodiscard]] std::string SafeDirectoryLister::get() const
{
    if (this->_dir == nullptr)
        return {};
    dirent* f_dirent = readdir(this->_dir);
    while (f_dirent != nullptr &&
        (this->_showHiddenFiles ? false : f_dirent->d_name[0] == '.')) {
        f_dirent = readdir(this->_dir);
        }
    if (f_dirent == nullptr)
        throw NoMoreFileException();
    return {f_dirent->d_name};
}
