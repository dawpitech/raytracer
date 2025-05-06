/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** ModuleLoader.hpp
*/

#ifndef MODULELOADER_HPP
    #define MODULELOADER_HPP

    #include "SafeDL.hpp"
    #include "RACIST/IModule.hpp"
    #include "RACIST/IObjectParser.hpp"

class ModuleLoader
{
    public:
        class Exception final : public std::exception
        {
            public:
                explicit Exception(const std::string& what = "ModuleLoader Exception") { this->_what = what; }
                [[nodiscard]] const char* what() const noexcept override { return this->_what.c_str(); }

            private:
                std::string _what;
        };

        static raytracer::generic::ModuleType getModuleType(const SafeDL::safeHandle& handle)
        {
            const auto func = reinterpret_cast<raytracer::generic::ModuleType(*)()>
                (dlsym(handle.get(), "getModuleType"));
            if (func == nullptr)
                throw Exception("Couldn't retrieve module info from shared library");
            return func();
        }

        static raytracer::generic::ModuleVersion getModuleVersion(const SafeDL::safeHandle& handle)
        {
            const auto func = reinterpret_cast<raytracer::generic::ModuleVersion(*)()>
                (dlsym(handle.get(), "getModuleVersion"));
            if (func == nullptr)
                throw Exception("Couldn't retrieve module info from shared library");
            return func();
        }

        static std::unique_ptr<raytracer::generic::IObjectParser> retrieveParser(const SafeDL::safeHandle& handle)
        {
            const auto func = reinterpret_cast<std::unique_ptr<raytracer::generic::IObjectParser>(*)()>
                (dlsym(handle.get(), "getObjectParser"));
            if (func == nullptr)
                throw Exception("Couldn't retrieve object parser from shared library");
            auto parser = func();
            return std::move(parser);
        }
};
#endif //MODULELOADER_HPP
