/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Raytracer.cpp
*/

#include <iostream>
#include <thread>

#include "Raytracer.hpp"

#include <plugins/SafeDirectoryLister.hpp>

#include "engine/renderers/PPMRenderer.hpp"
#include "plugins/ModuleLoader.hpp"

void raytracer::Raytracer::run()
{
    this->_renderer = std::make_unique<graphics::PPMRenderer>();

    this->_camera.render(_world, *this->_renderer);

    do {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    } while (!this->_renderer->shouldExit());
}

void raytracer::Raytracer::loadPlugins()
{
    const SafeDirectoryLister lister{"plugins", false};

    try {
        while (true) {
            auto handle = SafeDL::open("plugins/" + lister.get(), RTLD_LAZY);
            const auto version = ModuleLoader::getModuleVersion(handle);
            const auto type = ModuleLoader::getModuleType(handle);
            if (version >= generic::ModuleVersion::NOT_SUPPORTED || version == generic::ModuleVersion::UNKNOWN)
                return;
            switch (type) {
                case generic::ModuleType::PRIMITIVE:
                    {
                        const auto parser = ModuleLoader::retrieveParser(handle);

                        for (const auto& supportedObject : parser->getSupportedObjects())
                            this->_objectsParser.insert(std::make_pair(supportedObject, ModuleLoader::retrieveParser(handle)));

                        this->_pluginInventory.emplace_back(std::move(handle));
                    }
                default:
                    continue;
            }
        }
    } catch (SafeDirectoryLister::NoMoreFileException& e) {}
}

void raytracer::Raytracer::parseSceneConfig(const std::string& filepath)
{
    libconfig::Config cfg;

    // Read the file. If there is an error, report it and exit.
    try {
        cfg.readFile(filepath);
    } catch(const libconfig::FileIOException &fioex) {
        std::cerr << "I/O error while reading file." << std::endl;
        throw std::exception();
    } catch(const libconfig::ParseException &pex) {
        std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
                  << " - " << pex.getError() << std::endl;
        throw std::exception();
    }

    const libconfig::Setting& root = cfg.getRoot();
    const libconfig::Setting& objects = root.lookup("objects");

    try {
        const int objectsNumber = objects.getLength();
        std::cout << "[TRACE] Objects found: " << objectsNumber << std::endl;

        for (int i = 0; i < objectsNumber; i++) {
            const libconfig::Setting& obj = objects[i];
            const std::string obj_type = obj.lookup("type");

            std::clog << "[TRACE] Parsing object with type " << obj_type << std::endl;

            auto engineObject = this->_objectsParser.at(obj_type)->parseObject(obj);
            this->_world.add(engineObject);
        }
    } catch(const libconfig::SettingNotFoundException& nfex) {
        throw std::exception();
    } catch (const std::out_of_range& e) {
        throw std::exception();
    }
}
