/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** SafeDL.hpp
*/

#pragma once

#include <dlfcn.h>
#include <exception>
#include <memory>
#include <string>

class SafeDL
{
    public:
        class Exception final : public std::exception
        {
            public:
                explicit Exception(const std::string& what = "Couldn't open given shared library") { this->_what = what; }
                [[nodiscard]] const char* what() const noexcept override { return this->_what.c_str(); }

            private:
                std::string _what;
        };

        struct SafeDLDeleter
        {
            void operator()(void* handle) const
            {
                if (handle == nullptr || dlclose(handle) != 0)
                    throw Exception();
            }
        };
        typedef std::unique_ptr<void, SafeDLDeleter> safeHandle;

        static safeHandle make_safeHandle(void* handle) { return safeHandle(handle); }

        static safeHandle open(const std::string& filename, const int flags)
        {
            const std::string filename_r = "./" + filename;
            void* rawHandle = dlopen(filename_r.c_str(), flags);
            if (rawHandle == nullptr)
                throw Exception(dlerror());
            return make_safeHandle(rawHandle);
        }
};
