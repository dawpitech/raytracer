/*
** EPITECH PROJECT, 2025
** anal
** File description:
** IModule.hpp
*/

// ReSharper disable CppClassCanBeFinal
#pragma once

namespace raytracer::generic
{
    enum class ModuleType
    {
        UNKNOWN,
        PRIMITIVE,
        MATERIAL,
        OTHER,
    };

    enum class ModuleVersion
    {
        UNKNOWN,
        V1_0_0,
        NOT_SUPPORTED,
    };
}

extern "C" raytracer::generic::ModuleVersion getModuleVersion(void);
extern "C" raytracer::generic::ModuleType getModuleType(void);
