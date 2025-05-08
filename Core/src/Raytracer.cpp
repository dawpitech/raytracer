/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Raytracer.cpp
*/

#include <iostream>
#include <thread>

#include "Raytracer.hpp"
#include "engine/renderers/RendererFactory.hpp"
#include "parser/MasterParser.hpp"
#include "plugins/ModuleLoader.hpp"
#include "plugins/SafeDirectoryLister.hpp"

void raytracer::Raytracer::run()
{
    const std::string rendererName = "PPM";
    try {
        this->_renderer = graphics::RendererFactory::createRenderer(rendererName);
    } catch (graphics::RendererFactory::UnknownRendererException&) {
        std::cerr << "[ERR!] No valid renderer found with name: " << rendererName << std::endl;
        return;
    }

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

int raytracer::Raytracer::parseSceneConfig(const std::string& filepath)
{
    try {
        parser::MasterParser::parseScene(filepath, *this);
    } catch (parser::MasterParser::ParserException& pe) {
        std::cerr << "[ERR!] " << pe.what() << std::endl;
        return -1;
    } catch (generic::IObjectParser::ObjectParserException &ope) {
        std::cerr << "[ERR!] (plugin reported:) " << ope.what() << std::endl;
        return -1;
    }
    try {
        this->_camera.updateRenderingConfig();
    } catch (engine::Camera::CameraException&) {
        std::cerr << "[ERR!] Given configuration couldn't be rendered, are requirements matched ?" << std::endl;
        return -1;
    }
    return 0;
}

const std::map<std::string, std::unique_ptr<raytracer::generic::IObjectParser>>& raytracer::Raytracer::getObjectsParser() const
{
    return this->_objectsParser;
}

raytracer::engine::Scene& raytracer::Raytracer::getMainScene()
{
    return this->_world;
}

raytracer::engine::Camera& raytracer::Raytracer::getMainCamera()
{
    return this->_camera;
}
